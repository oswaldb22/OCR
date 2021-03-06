/*
TODO : Insert fancy header
*/

#include "bndBox.h"

bndBox bndBoxNew(ULONG x1, ULONG y1, ULONG x2, ULONG y2)
{
	bndBox box;
	box.x1 = x1;
	box.y1 = y1;
	box.x2 = x2;
	box.y2 = y2;
	box.mode = NONE;
	return box;
}

void bndBoxInit(bndBox *box, ULONG x1, ULONG y1, ULONG x2, ULONG y2) {
	box->x1 = x1;
	box->y1 = y1;
	box->x2 = x2;
	box->y2 = y2;
	box->mode = NONE;
}

void bndBoxFree(bndBox *box) {
	box->x1 = 0;
	box->y1 = 0;
	box->x2 = 0;
	box->y2 = 0;
	box->mode = 0;
}

ulong bndBoxGetWidth(const bndBox *box) {
	return box->x2 - box->x1 + 1;
}

ulong bndBoxGetHeight(const bndBox *box) {
	return box->y2 - box->y1 + 1;
}

void bndBoxDebugPrint(const bndBox *box) {
	printf("bndBox : (x:%lu,y:%lu) (x:%lu,y:%lu)",
		box->x1, box->y1, box->x2, box->y2);
	switch (box->mode) {

	case LINE:
		printf(" -> LINE\n");
		break;

	case CHAR:
		printf(" -> CHAR\n");
		break;

	default:
		printf("\n");
		break;
	}
}
