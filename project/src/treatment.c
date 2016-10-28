#include "treatment.h"

//TODO : here be demons #2
void invertImg(SDL_Surface *img){
	ulong wi = img->w;
	ulong he = img->h;
	for (ulong h = 0; h < he; h++){
                for (ulong w = 0; w < wi; w++){
                        Uint32 pix = getpixel(img, w, h);
                        Uint8 r = 0;
                        Uint8 g = 0;
                        Uint8 b = 0;
			SDL_GetRGB(pix, img->format, &r, &g, &b);
			r = 255 - r;
			g = 255 - g;
			b = 255 - b;
                        pix = SDL_MapRGB(img->format, r, g, b);
                        putpixel(img, w, h, pix);
                }
        }

}

void convertRgbToBmp(rgbMatrix *rgbM_in, SDL_Surface *surface_out){

	for (ulong h = 0; h < rgbGetHeight(rgbM_in); h++){
		for (ulong w = 0; w < rgbGetWidth(rgbM_in); w++){
			color c = rgbGetColorXY(rgbM_in, w, h);
			Uint8 r = c.r;
			Uint8 g = c.g;
			Uint8 b = c.b;
			Uint32 pix = SDL_MapRGB(surface_out->format, r, g, b);
			putpixel(surface_out, w, h, pix);
		}
	}
}

void convertBwToBmp(bwMatrix *bwM_in, SDL_Surface *surface_out){

        for (ulong h = 0; h < bwM_in->height; h++){
                for (ulong w = 0; w < bwM_in->width; w++){
                        uint val = bwMatrixGetValue(bwM_in, w, h);
                        Uint8 r = (!val) * 255;
                        Uint8 g = (!val) * 255;
                        Uint8 b = (!val) * 255;
                        Uint32 pix = SDL_MapRGB(surface_out->format, r, g, b);
                        putpixel(surface_out, w, h, pix);
                }
        }
}


void convertToBw(rgbMatrix *rgbM_in, bwMatrix *bwM_out, int threshold) {

	color c;
	for (ulong w = 0; w < rgbGetWidth(rgbM_in); w++)
		for (ulong h = 0; h < rgbGetHeight(rgbM_in); h++)
		{
			c = rgbGetColorXY(rgbM_in, w, h);
			float res = (float)c.r * 0.3 + (float)c.b * 0.11 + (float)c.g * 0.59;
			res = res < threshold ? 1 : 0;
			bwMatrixSetValue(bwM_out, w, h, (uint)res);
		}
}

void convertToRgb(bwMatrix *bwM_in, rgbMatrix *rgbM_out) {

	color c;
	uint val;
	for (ulong w = 0; w < bwM_in->width; w++)
		for (ulong h = 0; h < bwM_in->height; h++)
		{
			val = !bwMatrixGetValue(bwM_in, w, h) * 255;
			colorSetRGB(&c, val, val, val);
			rgbSetColorXY(rgbM_out, w, h, c);
		}
}

void drawBoundingBoxes(rgbMatrix *rgbM_in, bndBoxList *bndList_draw) {

	ulong maxW = rgbGetWidth(rgbM_in);
	ulong maxH = rgbGetHeight(rgbM_in);

	color c;
	colorSetRGB(&c, 0, 255, 0);

	for (ulong i = 0; i < bndList_draw->size; ++i)
	{
		bndBox charbox = bndList_draw->list[i];
		ulong hmin = charbox.y1 - 1;
		ulong hmax = charbox.y2 + 1;
		for (ulong w = charbox.x1 - 1; w < charbox.x2 + 1; ++w)
			if (0 < w && w < maxW) {
				if (0 < hmin)
					rgbSetColorXY(rgbM_in, w, hmin, c);
				if (hmax < maxH)
					rgbSetColorXY(rgbM_in, w, hmax, c);
			}

		ulong wmin = charbox.x1 - 1;
		ulong wmax = charbox.x2 + 1;
		for (ulong h = charbox.y1 - 1; h < charbox.y2 + 1; ++h)
			if (0 < h && h < maxH) {
				if (0 < wmin)
					rgbSetColorXY(rgbM_in, wmin, h, c);
				if (wmax < maxW)
					rgbSetColorXY(rgbM_in, wmax, h, c);
			}

	}
}

// TODO : here be demons, to modify as soon as possible
void drawBoundingBoxesBw(bwMatrix *rgbM_in, bndBoxList *bndList_draw) {

        ulong maxW = rgbM_in->width;
        ulong maxH = rgbM_in->height;

        for (ulong i = 0; i < bndList_draw->size; ++i)
        {
                bndBox charbox = bndList_draw->list[i];
                ulong hmin = charbox.y1 - 1;
                ulong hmax = charbox.y2 + 1;
                for (ulong w = charbox.x1 - 1; w < charbox.x2 + 1; ++w)
                        if (0 < w && w < maxW) {
                                if (0 < hmin)
                                        bwMatrixSetValue(rgbM_in, w, hmin, 1);
                                if (hmax < maxH)
                                        bwMatrixSetValue(rgbM_in, w, hmax, 1);
                        }

                ulong wmin = charbox.x1 - 1;
                ulong wmax = charbox.x2 + 1;
                for (ulong h = charbox.y1 - 1; h < charbox.y2 + 1; ++h)
                        if (0 < h && h < maxH) {
                                if (0 < wmin)
                                        bwMatrixSetValue(rgbM_in, wmin, h, 1);
                                if (wmax < maxW)
                                        bwMatrixSetValue(rgbM_in, wmax, h, 1);
                        }

        }
}


void removeWhiteSpaces(bwMatrix *bwM_noModify, bndBox *box_toResize) {

	if (bwMatrixGetValue(bwM_noModify, box_toResize->x1, box_toResize->y1) && bwMatrixGetValue(bwM_noModify, box_toResize->x2, box_toResize->y2))
		return;

	if (bwMatrixGetValue(bwM_noModify, box_toResize->x2, box_toResize->y1) && bwMatrixGetValue(bwM_noModify, box_toResize->x1, box_toResize->y2))
		return;

	int getOut = 0;
	//Left
	while (!getOut) {
		if (box_toResize->x1 == box_toResize->x2)
			break;
		for (ulong i = box_toResize->y1; i < box_toResize->y2; ++i) {
			if (bwMatrixGetValue(bwM_noModify, box_toResize->x1, i) != 0) {
				getOut = 1;
				break;
			}
		}
		if (!getOut)
			++box_toResize->x1;
	}
	//Right
	getOut = 0;
	while (!getOut) {
		if (box_toResize->x1 == box_toResize->x2)
			break;
		for (ulong i = box_toResize->y1; i < box_toResize->y2; ++i) {
			if (bwMatrixGetValue(bwM_noModify, box_toResize->x2, i) != 0) {
				getOut = 1;
				break;
			}
		}
		if (!getOut)
			--box_toResize->x2;
	}
	//Top
	getOut = 0;
	while (!getOut) {
		if (box_toResize->y1 == box_toResize->y2)
			break;
		for (ulong i = box_toResize->x1; i < box_toResize->x2; ++i) {
			if (bwMatrixGetValue(bwM_noModify, i, box_toResize->y1) != 0) {
				getOut = 1;
				break;
			}
		}
		if (!getOut)
			++box_toResize->y1;

	}
	//Bottom
	getOut = 0;
	while (!getOut) {
		if (box_toResize->y1 == box_toResize->y2)
			break;
		for (ulong i = box_toResize->x1; i < box_toResize->x2; ++i) {
			if (bwMatrixGetValue(bwM_noModify, i, box_toResize->y2) != 0) {
				getOut = 1;
				break;
			}
		}
		if (!getOut)
			--box_toResize->y2;
	}
}

void cropUsingBox(bwMatrix *bwM_toCrop, bwMatrix *bwM_res, bndBox *box) {

	ulong width = bndBoxGetWidth(box);
	ulong height = bndBoxGetHeight(box);

	for (ulong w = 0; w < width; ++w)
		for (ulong h = 0; h < height; ++h) {
			uint newval = bwMatrixGetValue(bwM_toCrop, box->x1 + w, box->y1 + h);
			bwMatrixSetValue(bwM_res, w, h, newval);
			//bwMatrixPrintCompact(bwM_res, Advanced);
		}
}

void getLines(bwMatrix *bwM_block, bndBoxList *bndList_out, bndBoxList *bndList_draw, ulong originW, ulong originH) {

	int buildingline = 0;
	ulong h = 0;
	ulong start = 0;
	
	//For each row in the Matrix
	while (h < bwM_block->height)
	{
		ulong w = 0;
		while (w < bwM_block->width) {
			//If we encounter a line and are not building one, start a new line
			if (buildingline == 0 && bwMatrixGetValue(bwM_block, w, h) == 1) {
				start = h;
				buildingline = 1;
				break;
			}
			//If we are building a line
			if (buildingline == 1) {
				//If we encounter something, this is part of our line so go to next row
				if (bwMatrixGetValue(bwM_block, w, h) == 1)
					break;
				//If we reach the end of the row without encountering anything, end the line
				if (w == bwM_block->width - 1) {
					bndBox newline;
					bndBoxInit(&newline, 0, start, w, h - 1);
					removeWhiteSpaces(bwM_block, &newline);
					bndBoxListAdd(bndList_out, newline);

					//Gives modified version in global coordinates to draw
					newline.x1 += originW;
					newline.x2 += originW;
					newline.y1 += originH;
					newline.y2 += originH;
					newline.mode = LINE;
					bndBoxDebugPrint(&newline);
					bndBoxListAdd(bndList_draw, newline);

					buildingline = 0;
					bndBoxFree(&newline);
					break;
				}
			}
			w++;
		}
		h++;
	}
}

void getChars(bwMatrix *bwM_line, bndBoxList *bndList_out, bndBoxList *bndList_draw, ulong originW, ulong originH) {

	int buildingchar = 0;
	ulong w = 0;
	ulong start = 0;
	//For each column in the Matrix
	while (w < bwM_line->width)
	{
		ulong h = 0;
		while (h < bwM_line->height) {
			//If we encounter a char and are not building one, start a new char
			if (buildingchar == 0 && bwMatrixGetValue(bwM_line, w, h) == 1) {
				start = w;
				buildingchar = 1;
				break;
			}
			//If we are building a char
			if (buildingchar == 1) {
				//If we encounter something, this is part of our char so go to next column
				if (bwMatrixGetValue(bwM_line, w, h) == 1)
					break;
				//If we reach the end of the column without encountering anything, end the char
				if (h == bwM_line->height - 1) {
					bndBox newchar;
					bndBoxInit(&newchar, start, 0, w - 1, h);
					removeWhiteSpaces(bwM_line, &newchar);
					bndBoxListAdd(bndList_out, newchar);

					//Gives modified version in global coordinates to draw
					newchar.x1 += originW;
					newchar.x2 += originW;
					newchar.y1 += originH;
					newchar.y2 += originH;
					newchar.mode = CHAR;
					bndBoxListAdd(bndList_draw, newchar);

					buildingchar = 0;
					bndBoxFree(&newchar);
					break;
				}
			}
			h++;
		}
		if (w == bwM_line->width - 1 && buildingchar == 1) {
			bndBox newchar;
			bndBoxInit(&newchar, start, 0, w, bwM_line->height - 1);
			removeWhiteSpaces(bwM_line, &newchar);
			bndBoxListAdd(bndList_out, newchar);

			//Gives modified version in global coordinates to draw
			newchar.x1 += originW;
			newchar.x2 += originW;
			newchar.y1 += originH;
			newchar.y2 += originH;
			newchar.mode = CHAR;
			bndBoxListAdd(bndList_draw, newchar);

			buildingchar = 0;
			bndBoxFree(&newchar);
		}
		w++;
	}
}

void getEverything(bwMatrix *bwM_block_in, bwMatrixList *bwMList_lines_out, bwMatrixList *bwMList_chars_out, bndBoxList *bndList_draw_lines, bndBoxList *bndList_draw_chars) {

	UNUSED(bwMList_lines_out);
	UNUSED(bwMList_chars_out);

	bndBoxList lineList;
	bndBoxListInit(&lineList);
	getLines(bwM_block_in, &lineList, bndList_draw_lines, 0, 0);
	printf("Lines : %lu \n", bndList_draw_lines->size);
	
	for (ulong i = 0; i < lineList.size; i++)
	{
		bwMatrix line;
		bndBox linebox = lineList.list[i];
		bwMatrixInit(&line, bndBoxGetWidth(&linebox), bndBoxGetHeight(&linebox));
		cropUsingBox(bwM_block_in, &line, &linebox);
		//bwMatrixListAdd(bwMList_lines_out, line);

		bndBoxList charList;
		bndBoxListInit(&charList);
		getChars(&line, &charList, bndList_draw_chars, linebox.x1, linebox.y1);

		for (ulong j = 0; j < charList.size; j++)
		{
			bwMatrix charac;
			bndBox charbox = charList.list[j];
			bwMatrixInit(&charac, bndBoxGetWidth(&charbox), bndBoxGetHeight(&charbox));
			cropUsingBox(&line, &charac, &charbox);
			//bwMatrixListAdd(bwMList_chars_out, charac);

			bwMatrixFree(&charac);
		}

		bndBoxListFree(&charList);
		bwMatrixFree(&line);
	}

	bndBoxListFree(&lineList);

	//bwMatrixListDebugPrint(bwMList_lines_out);
	//bwMatrixListDebugPrint(bwMList_chars_out);
}
