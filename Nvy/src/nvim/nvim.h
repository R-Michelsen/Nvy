#pragma once

enum NvimMethod : uint8_t {
	vim_get_api_info = 0,
	nvim_input = 1,
	nvim_input_mouse = 2
};
constexpr const char *NVIM_METHOD_NAMES[] {
	"nvim_get_api_info",
	"nvim_input",
	"nvim_input_mouse"
};
enum NvimOutboundNotification : uint8_t {
	nvim_ui_attach = 0,
	nvim_ui_try_resize_grid = 1
};
constexpr const char *NVIM_OUTBOUND_NOTIFICATION_NAMES[] {
	"nvim_ui_attach",
	"nvim_ui_try_resize_grid"
};
enum class MouseButton {
	Left,
	Right,
	Middle,
	Wheel
};
enum class MouseAction {
	Press,
	Drag,
	Release,
	MouseWheelUp,
	MouseWheelDown,
	MouseWheelLeft,
	MouseWheelRight
};

struct Nvim {
	int64_t current_msg_id;
	std::vector<NvimMethod> msg_id_to_method;

	HWND hwnd;
	HANDLE stdin_read;
	HANDLE stdin_write;
	HANDLE stdout_read;
	HANDLE stdout_write;
	PROCESS_INFORMATION process_info;
};

void NvimInitialize(Nvim *nvim, HWND hwnd);
void NvimShutdown(Nvim *nvim);

void NvimSendUIAttach(Nvim *nvim, int grid_rows, int grid_cols);
void NvimSendResize(Nvim *nvim, int grid_rows, int grid_cols);
void NvimSendInput(Nvim *nvim, char input_char);
void NvimSendInput(Nvim *nvim, const char* input_chars);
void NvimSendInput(Nvim *nvim, int virtual_key);
void NvimSendMouseInput(Nvim *nvim, MouseButton button, MouseAction action, int mouse_row, int mouse_col);