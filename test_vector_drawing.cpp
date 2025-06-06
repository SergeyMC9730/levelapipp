/**
 *  LevelAPI - Geometry Dash level cacher with search functionality and more.
    Copyright (C) 2025  Sergei Baigerov

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/

#include <blend2d.h>
#include "blend2d/api.h"
#include "blend2d/format.h"
#include "blend2d/rgba.h"
#include "tests.h"

#include <iostream>

void LevelAPI::Tests::testVectorDrawing() {
    std::cout << "testing vector drawing\n";

    BLImage img(781, 317, BL_FORMAT_PRGB32);
    BLContext ctx(img);

    ctx.fillAll(BLRgba32(0xFF000000)); // ARGB

    ctx.setFillStyle(BLRgba32(0xFF242429));
    ctx.fillRoundRect(0, 0, img.width(), img.height(), 8.f);

    ctx.setStrokeStyle(BLRgba32(0xFF424247));
    ctx.setStrokeWidth(3.f);
    ctx.strokeRoundRect(0, 0, img.width(), img.height(), 8.f);

    ctx.setFillStyle(BLRgba32(rand() % 255, rand() % 255, rand() % 255, 0xFF));
    ctx.fillRoundRect(0, 0, 8, img.height(), 8.f);
    ctx.fillRect(2.5, 0, 5.5, img.height());

    BLFontFace semiBoldFace;
    BLResult res = semiBoldFace.createFromFile("static/OpenSans-SemiBold.ttf");
    if (res != BL_SUCCESS) {
        std::cout << "failed to load static/OpenSans-SemiBold.ttf\n";
        printf("reason: %d\n", (int)res);
    } else {
        BLFont semiBold;
        semiBold.createFromFace(semiBoldFace, 23.f);

        ctx.setFillStyle(BLRgba32(0xFFFFFFFF));
        ctx.fillUtf8Text(BLPoint(25 + 1, 40), semiBold, "New Level");

        BLFontFace boldFace;
        res = boldFace.createFromFile("static/OpenSans-Bold.ttf");
        if (res != BL_SUCCESS) {
            std::cout << "failed to load static/OpenSans-Bold.ttf\n";
            printf("reason: %d\n", (int)res);
        } else {
            BLFont bold;
            bold.createFromFace(boldFace, 20.f);

            ctx.fillUtf8Text(BLPoint(25 + 1, 81), bold, "New 2.2 level was been uploaded to the server.");
        }
    }

    ctx.end();

    img.writeToFile("vtest.png");

    std::cout << "testing complete\n";
}
