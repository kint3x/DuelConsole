import os
from PIL import Image

# -----------------------------
# CONFIG
# -----------------------------
ROOT_DIR = "."
OUTPUT_HEADER = "resources.hpp"

SUPPORTED_EXT = (".bmp", ".png", ".jpg", ".jpeg", ".webp")

gen_dir = os.path.join(os.path.dirname(os.path.abspath(__file__)), "generated")
os.makedirs(gen_dir, exist_ok=True)
# -----------------------------
# RGB conversion
# -----------------------------
def rgb888_to_rgb565(r, g, b):
    return ((r >> 3) << 11) | ((g >> 2) << 5) | (b >> 3)


# -----------------------------
# sanitize name
# -----------------------------
def sanitize(name):
    return name.replace(" ", "_").replace("-", "_").replace(".", "_")


# -----------------------------
# convert image -> framebuffer_t
# -----------------------------
def convert_image(img_path, out_c_path, var_name):
    img = Image.open(img_path)

    has_alpha = img.mode in ("RGBA", "LA") or ("transparency" in img.info)

    if has_alpha:
        img = img.convert("RGBA")
    else:
        img = img.convert("RGB")

    width, height = img.size
    pixels = list(img.getdata())

    with open(out_c_path, "w") as f:
        # engine include (defines TRANSPARENT_COLOR)
        f.write("#include <ICQEngine/include/ICQTypes.hpp>\n\n")

        f.write(f"static uint16_t {var_name}_pixels[] = {{\n")

        for y in range(height):
            row = []
            for x in range(width):
                i = y * width + x

                if has_alpha:
                    r, g, b, a = pixels[i]

                    if a < 128:
                        # USE ENGINE MACRO (NOT PYTHON CONSTANT)
                        rgb = "TRANSPARENT_COLOR"
                    else:
                        rgb = f"0x{rgb888_to_rgb565(r, g, b):04x}"
                else:
                    r, g, b = pixels[i]
                    rgb = f"0x{rgb888_to_rgb565(r, g, b):04x}"

                row.append(str(rgb))

            f.write("    " + ",".join(row) + ",\n")

        f.write("};\n\n")

        f.write(f"framebuffer_t {var_name} = {{\n")
        f.write(f"    {width},\n")
        f.write(f"    {height},\n")
        f.write(f"    {var_name}_pixels\n")
        f.write("};\n")


# -----------------------------
# process resources folder
# -----------------------------
def process_resources(folder):
    header = []
    c_files = []

    for file in os.listdir(folder):
        if file.lower().endswith(SUPPORTED_EXT):
            path = os.path.join(folder, file)

            name = sanitize(os.path.splitext(file)[0])

            c_file = f"{name}.cpp"

            c_path = os.path.join(gen_dir, c_file)

            print(f"Converting {file} -> {c_file}")

            convert_image(path, c_path, name)

            c_files.append(c_file)
            header.append(f"extern framebuffer_t {name};\n")

    # write header
    header_path = os.path.join(gen_dir, OUTPUT_HEADER)

    with open(header_path, "w") as f:
        f.write("#pragma once\n\n")
        f.write("#include <ICQEngine/include/ICQTypes.hpp>\n\n")

        for line in header:
            f.write(line)

        f.write("\n// Generated files:\n")
        for c in c_files:
            f.write(f"// {c}\n")


# -----------------------------
# scan project
# -----------------------------
def main():
    for root, dirs, files in os.walk(ROOT_DIR):
        for d in dirs:
            if d.lower() == "resources":
                folder = os.path.join(root, d)
                print(f"\nProcessing: {folder}")
                process_resources(folder)


if __name__ == "__main__":
    main()