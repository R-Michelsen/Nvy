#pragma once
#include <windows.h>
#include <functional>
#include "mpack/mpack.h"

void MPackSendRequest(Nvim *nvim, NvimMethod method, std::function<void(mpack_writer_t *writer)> Callback = nullptr) {
	char *data;
	size_t size;
	mpack_writer_t writer;
	mpack_writer_init_growable(&writer, &data, &size);

	mpack_start_array(&writer, 4);
	mpack_write_i64(&writer, 0);
	mpack_write_i64(&writer, nvim->current_msg_id++);
	nvim->msg_id_to_method.push_back(method);
	mpack_write_cstr(&writer, NVIM_METHOD_NAMES[method]);

	if (Callback) {
		Callback(&writer);
	}
	else {
		mpack_start_array(&writer, 0);
		mpack_finish_array(&writer);
	}

	mpack_finish_array(&writer);

	if (mpack_writer_destroy(&writer) != mpack_ok) {
		fprintf(stderr, "An error occurred encoding the data!\n");
		return;
	}

	DWORD bytes_written;
	BOOL b = WriteFile(nvim->stdin_write, data, static_cast<DWORD>(size), &bytes_written, nullptr);
	assert(b);

	free(data);
}

void MPackSendNotification(Nvim *nvim, NvimOutboundNotification notification, std::function<void(mpack_writer_t *writer)> Callback = nullptr) {
	char *data;
	size_t size;
	mpack_writer_t writer;
	mpack_writer_init_growable(&writer, &data, &size);

	mpack_start_array(&writer, 3);
	mpack_write_i64(&writer, 2);
	mpack_write_cstr(&writer, NVIM_OUTBOUND_NOTIFICATION_NAMES[notification]);

	if (Callback) {
		Callback(&writer);
	}
	else {
		mpack_start_array(&writer, 0);
		mpack_finish_array(&writer);
	}

	mpack_finish_array(&writer);

	if (mpack_writer_destroy(&writer) != mpack_ok) {
		fprintf(stderr, "An error occurred encoding the data!\n");
		return;
	}

	DWORD bytes_written;
	BOOL b = WriteFile(nvim->stdin_write, data, static_cast<DWORD>(size), &bytes_written, nullptr);
	assert(b);

	free(data);
}