#pragma once
#include <cassert>
#include "mpack/mpack.h"

inline int MPackIntFromArray(mpack_node_t arr, int index) {
	return static_cast<int>(mpack_node_array_at(arr, index).data->value.i);
}

inline bool MPackMatchString(mpack_node_t node, const char *str_to_match) {
	assert(node.data->type == mpack_type_str);
	const char *str = mpack_node_str(node);
	return strncmp(str, str_to_match, mpack_node_strlen(node)) == 0;
}

enum class MPackMessageType {
	Response = 1,
	Notification = 2
};
struct MPackResponse {
	mpack_node_t error;
	int64_t msg_id;
};
struct MPackNotification {
	mpack_node_t name;
};
struct MPackMessageResult {
	MPackMessageType type;
	mpack_node_t params;
	union {
		MPackResponse response;
		MPackNotification notification;
	};
};
inline MPackMessageResult MPackExtractMessageResult(mpack_tree_t *tree) {
	mpack_node_t root = mpack_tree_root(tree);
	assert(mpack_node_array_at(root, 0).data->type == mpack_type_uint);

	MPackMessageType message_type = static_cast<MPackMessageType>(mpack_node_array_at(root, 0).data->value.i);
	if (message_type == MPackMessageType::Response) {
		assert(mpack_node_array_at(root, 1).data->type == mpack_type_uint);
		assert(mpack_node_array_at(root, 2).data->type == mpack_type_nil);

		return MPackMessageResult {
			.type = message_type,
			.params = mpack_node_array_at(root, 3),
			.response {
				.error = mpack_node_array_at(root, 2),
				.msg_id = mpack_node_array_at(root, 1).data->value.i
			}
		};
	}
	else if (message_type == MPackMessageType::Notification) {
		assert(mpack_node_array_at(root, 1).data->type == mpack_type_str);
		assert(mpack_node_array_at(root, 2).data->type == mpack_type_array);

		return MPackMessageResult {
			.type = message_type,
			.params = mpack_node_array_at(root, 2),
			.notification = {
				.name = mpack_node_array_at(root, 1)
			}
		};
	}

	return MPackMessageResult {};
}