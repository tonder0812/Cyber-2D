#include "pch.h"
#include "Python.h"
#include "Python/PyInput.h"
#include "Core/Application.h"
#include "Core/Input.h"
#include "PyGLM/types/types.h"
#include "glm/glm.hpp"

static PyObject* InputTypeGetMousePosition() {
	vec<2, float>* pos = (vec<2, float> *)PyObject_CallObject(Cyber::Application::Get().GetPyGLM_Vec2(), NULL);
	pos->super_type = Cyber::Input::GetMousePositionViewport();
	return (PyObject*)pos;
}

static const std::unordered_map<std::string, int>s_KeyCodes = {
	{" ",               32},
	{"SPACE",           32},
	{"'",               39},  /* ' */
	{"APOSTROPHE",      39},  /* ' */
	{","    ,           44},  /* , */
	{"COMMA",           44},  /* , */
	{"-",               45},  /* - */
	{"MINUS",           45},  /* - */
	{".",               46},  /* . */
	{"PERIOD",          46},  /* . */
	{"/",               47},  /* / */
	{"SLASH",           47},  /* / */
	{"0",               48},
	{"1",               49},
	{"2",               50},
	{"3",               51},
	{"4",               52},
	{"5",               53},
	{"6",               54},
	{"7",               55},
	{"8",               56},
	{"9",               57},
	{";",               59},  /* ; */
	{"SEMICOLON",       59},  /* ; */
	{"=",               61},  /* = */
	{"EQUAL",           61},  /* = */
	{"A",               65},
	{"B",               66},
	{"C",               67},
	{"D",               68},
	{"E",               69},
	{"F",               70},
	{"G",               71},
	{"H",               72},
	{"I",               73},
	{"J",               74},
	{"K",               75},
	{"L",               76},
	{"M",               77},
	{"N",               78},
	{"O",               79},
	{"P",               80},
	{"Q",               81},
	{"R",               82},
	{"S",               83},
	{"T",               84},
	{"U",               85},
	{"V",               86},
	{"W",               87},
	{"X",               88},
	{"Y",               89},
	{"Z",               90},
	{"[",               91}, /* [ */
	{"LEFT_BRACKET",    91}, /* [ */
	{"\\",              92}, /* \ */
	{"BACKSLASH",       92}, /* \ */
	{"]",               93}, /* ] */
	{"RIGHT_BRACKET",   93}, /* ] */
	{"`",               96},/* ` */
	{"GRAVE_ACCENT",    96},/* ` */
	{"WORLD_1",         161},/* non-US #1 */
	{"WORLD_2",         162},/* non-US #2 */
	/* Function keys */
	{"ESC",             256},
	{"ESCAPE",          256},
	{"ENTER",           257},
	{"TAB",             258},
	{"BACKSPACE",       259},
	{"INSERT",          260},
	{"DELETE",          261},
	{"RIGHT",           262},
	{"LEFT",            263},
	{"DOWN",            264},
	{"UP",              265},
	{"PAGE_UP",         266},
	{"PAGE_DOWN",       267},
	{"HOME",            268},
	{"END",             269},
	{"CAPS_LOCK",       280},
	{"SCROLL_LOCK",     281},
	{"NUM_LOCK",        282},
	{"PRINT_SCREEN",    283},
	{"PAUSE",           284},
	{"F1",              290},
	{"F2",              291},
	{"F3",              292},
	{"F4",              293},
	{"F5",              294},
	{"F6",              295},
	{"F7",              296},
	{"F8",              297},
	{"F9",              298},
	{"F10",             299},
	{"F11",             300},
	{"F12",             301},
	{"F13",             302},
	{"F14",             303},
	{"F15",             304},
	{"F16",             305},
	{"F17",             306},
	{"F18",             307},
	{"F19",             308},
	{"F20",             309},
	{"F21",             310},
	{"F22",             311},
	{"F23",             312},
	{"F24",             313},
	{"F25",             314},
	{"KP_0",            320},
	{"KP_1",            321},
	{"KP_2",            322},
	{"KP_3",            323},
	{"KP_4",            324},
	{"KP_5",            325},
	{"KP_6",            326},
	{"KP_7",            327},
	{"KP_8",            328},
	{"KP_9",            329},
	{"KP_DECIMAL",      330},
	{"KP_DIVIDE",       331},
	{"KP_MULTIPLY",     332},
	{"KP_SUBTRACT",     333},
	{"KP_ADD",          334},
	{"KP_ENTER",        335},
	{"KP_EQUAL",        336},
	{"LEFT_SHIFT",      340},
	{"LEFT_CONTROL",    341},
	{"LEFT_ALT",        342},
	{"LEFT_SUPER",      343},
	{"RIGHT_SHIFT",     344},
	{"RIGHT_CONTROL",   345},
	{"RIGHT_ALT",       346},
	{"RIGHT_SUPER",     347},
	{"MENU",            348}
};
static const std::unordered_map<std::string, int>s_MouseCodes = {
	{"1"     ,0},
	{"2"     ,1},
	{"3"     ,2},
	{"4"     ,3},
	{"5"     ,4},
	{"6"     ,5},
	{"7"     ,6},
	{"8"     ,7},
	{"LEFT"  ,0},
	{"RIGHT" ,1},
	{"MIDDLE",2}
};

static PyObject* InputTypeGetMouseX() {
	return PyFloat_FromDouble(Cyber::Input::GetMouseXViewport());
}

static PyObject* InputTypeGetMouseY() {
	return PyFloat_FromDouble(Cyber::Input::GetMouseYViewport());
}

static PyObject* InputTypeIsKeyPressed(PyObject* self, PyObject* args) {
	const char* str;
	if (!PyArg_ParseTuple(args, "s", &str)) {
		return NULL;
	}
	auto tmp = s_KeyCodes.find(str);
	if (tmp == s_KeyCodes.end())
		Py_RETURN_FALSE;

	if (Cyber::Input::IsKeyPressed(tmp->second)) {
		Py_RETURN_TRUE;
	}
	else {
		Py_RETURN_FALSE;
	}
}
static PyObject* InputTypeIsMouseButtonPressed(PyObject* self, PyObject* args) {
	const char* str;
	if (!PyArg_ParseTuple(args, "s", &str)) {
		return NULL;
	}
	auto tmp = s_MouseCodes.find(str);
	if (tmp == s_KeyCodes.end())
		Py_RETURN_FALSE;

	if (Cyber::Input::IsMouseButtonPressed(tmp->second)) {
		Py_RETURN_TRUE;
	}
	else {
		Py_RETURN_FALSE;
	}
}


static PyMethodDef InputMethods[] = {
	{"GetMousePosition", (PyCFunction)InputTypeGetMousePosition, METH_STATIC | METH_NOARGS,
	 "Return a glm.vec2 with the mouse position relative to viewport with 0,0 at the top left"
	},
	{"GetMouseX", (PyCFunction)InputTypeGetMouseX, METH_STATIC | METH_NOARGS,
	 "Return a float with the mouse X position relative to viewport with 0 at the left"
	},
	{"GetMouseY", (PyCFunction)InputTypeGetMouseY, METH_STATIC | METH_NOARGS,
	 "Return a float with the mouse Y position relative to viewport with 0 at the top"
	},
	{"IsKeyPressed", (PyCFunction)InputTypeIsKeyPressed, METH_STATIC | METH_VARARGS,
	 "Return if a key is curently held down"
	},
	{"IsMouseButtonPressed", (PyCFunction)InputTypeIsMouseButtonPressed, METH_STATIC | METH_VARARGS,
	 "Return if a mouse button is curently pressed"
	},
	{NULL}  /* Sentinel */
};

PyTypeObject InputType =
{
	PyObject_HEAD_INIT(NULL)
		"Cyber.Input",						// tp_name
		sizeof(InputObject),				// tp_basicsize 
		0,									// tp_itemsize 
		0,									// tp_dealloc 
		0,									// tp_print 
		0,									// tp_getattr 
		0,									// tp_setattr 
		0,									// tp_reserved 
		0,									// tp_repr 
		0,									// tp_as_number 
		0,									// tp_as_sequence 
		0,									// tp_as_mapping 
		0,									// tp_hash  
		0,									// tp_call 
		0,									// tp_str 
		0,									// tp_getattro 
		0,									// tp_setattro 
		0,									// tp_as_buffer 
		Py_TPFLAGS_DEFAULT ,				// tp_flags 
		"Input class to group all input functions",	// tp_doc 
		0,									// tp_traverse 
		0,									// tp_clear 
		0,									// tp_richcompare 
		0,									// tp_weaklistoffset 
		0,									// tp_iter 
		0,									// tp_iternext 
		InputMethods,						// tp_methods 
		0,									// tp_members 
		0,									// tp_getset 
		0,									// tp_base 
		0,									// tp_dict 
		0,									// tp_descr_get 
		0,									// tp_descr_set 
		0,									// tp_dictoffset 
		0,									// tp_init 
		0,									// tp_alloc 
		0,									// tp_new 
};
