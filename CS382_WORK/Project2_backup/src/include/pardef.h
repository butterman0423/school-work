#ifndef PARSER_DEF
#define PARSER_DEF

enum FUNC {
	ADD = 0,
	SUB,
	ORR,
	AND,
	LBS,
	RBS,
	XOR,
	NOT,
	//SET_N,
	//SET_R,
	STORE,
	LOAD,
	FLASH,
	FLUSH,
	DELAY,
	SET
};

typedef struct {
	enum FUNC f;

	int dreg;
	char dlabel[10];

	int mreg;
	int nreg;
	int bits;
} Command;

#endif
