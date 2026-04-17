import os
from PIL import Image, ImageFont, ImageDraw

FONT_DIR = "./fonts"
OUTPUT_DIR = "ICQEngine/resources/fonts"

FONT_SIZES = [8, 12, 18, 24]

ASCII_START = 33
ASCII_END = 126

CHARS = [chr(i) for i in range(ASCII_START, ASCII_END + 1)]

FONTS_H_FILE = "ICQEngine/resources/fonts/fonts.hpp"
FONTS_CPP_FILE = "ICQEngine/resources/fonts/fonts.cpp"

FONT_ENUM_NAMES = []

def sanitize(name):
    return name.replace(" ", "_").replace(".", "_")


def render(font, char, size):
    img = Image.new("L", (size, size), 0)
    draw = ImageDraw.Draw(img)
    draw.text((0, 0), char, font=font, fill=255)
    return img


def to_binary(img):
    w, h = img.size
    px = list(img.getdata())
    return [1 if px[y*w + x] > 128 else 0 for y in range(h) for x in range(w)]


def generate(ttf_path):
    name = sanitize(os.path.splitext(os.path.basename(ttf_path))[0])
    base = os.path.join(OUTPUT_DIR, name)
    os.makedirs(base, exist_ok=True)

    for size in FONT_SIZES:
        font = ImageFont.truetype(ttf_path, size)

        glyph_data = []

        for c in CHARS:
            img = render(font, c, size)
            glyph_data.extend(to_binary(img))

        out_file = os.path.join(base, f"{name}_{size}.cpp")

        with open(out_file, "w") as f:
            f.write("#include <ICQEngine/include/ICQTypes.hpp>\n\n")

            total = len(glyph_data)

            f.write(f"static uint8_t font_{name}_{size}_pixels[{total}] = {{\n")

            for i, v in enumerate(glyph_data):
                if i % (size * size) == 0:
                    f.write("    ")

                f.write(f"{v},")

                if (i + 1) % (size * size) == 0:
                    f.write("\n")

            f.write("};\n\n")

            f.write(f"fontmap_t font_{name}_{size} = {{\n")
            f.write(f"    {size},\n")
            f.write(f"    font_{name}_{size}_pixels\n")
            f.write("};\n")

        with open(FONTS_H_FILE, "a") as f:
            f.write(f"extern fontmap_t font_{name}_{size};\n")


def main():
    global FONT_ENUM_NAMES
    with open(FONTS_H_FILE, "w") as f:
        f.write(f"#ifndef __FONTS_HPP\n")
        f.write(f"#define __FONTS_HPP\n")
        f.write(f"#include <ICQEngine/include/ICQTypes.hpp>\n\n")

    for file in os.listdir(FONT_DIR):
        if file.lower().endswith(".ttf"):
            generate(os.path.join(FONT_DIR, file))
            FONT_ENUM_NAMES.append(sanitize(os.path.splitext(os.path.basename(os.path.join(FONT_DIR, file)))[0]))

    with open(FONTS_H_FILE, "a") as f:
        f.write("enum FONT_TYPE{\n")
        for name in FONT_ENUM_NAMES:
            f.write(f"\tFONT_{name.upper()},\n")
        f.write("\tFONT_COUNT\n")
        f.write("};\n")

        f.write("enum FONT_SIZE{\n")
        for size in FONT_SIZES:
            f.write(f"\tFONT_SIZE_{size},\n")
        f.write("\tFONT_SIZE_COUNT\n")
        f.write("};\n")

        f.write("\nextern fontmap_t* FONT_REGISTRY[FONT_COUNT][FONT_SIZE_COUNT];\n\n")
        f.write(f"#endif //__FONTS_HPP\n")

    with open(FONTS_CPP_FILE, "w") as f:
        f.write("#include <ICQEngine/resources/fonts/fonts.hpp>\n\n")
        f.write("fontmap_t* FONT_REGISTRY[FONT_COUNT][FONT_SIZE_COUNT] = {\n")
        
        for name in FONT_ENUM_NAMES:
            f.write("\t{ //")
            f.write(f"{name}\n")
            for size in FONT_SIZES:
                f.write(f"\t\t&font_{name}_{size},\n")
            f.write("\t},\n")
        f.write("};\n")

if __name__ == "__main__":
    main()