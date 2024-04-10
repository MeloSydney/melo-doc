

//TODO - kernel input

struct input_event {
	struct timeval time;
	__u16 type;
	__u16 code;
	__s32 value;
};


evbit
keybit
relbit

input_handler

input_core

input_driver