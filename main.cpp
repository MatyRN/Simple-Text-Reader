#define WINVER 0x0501          // Windows XP
#define _WIN32_WINNT 0x0501
#include <FL/Fl.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Text_Editor.H>
#include <FL/Fl_Menu_Bar.H>
#include <FL/Fl_File_Chooser.H>
#include <FL/fl_ask.H>
#include <fstream>
#include <sstream>

Fl_Text_Buffer* textbuf = nullptr;
std::string current_file_path;

///========== NEW FILE ===========///
void new_file_cb(Fl_Widget*, void*) {
    textbuf->text("");
    current_file_path.clear();
}

///========== OPEN FILE ==========///
void open_file_cb(Fl_Widget*, void*) {
    const char* filepath = fl_file_chooser("Abrir archivo de texto", "*.txt", NULL);
    if (!filepath) return;

    std::ifstream file(filepath);
    if (!file) {
        fl_alert("No se pudo abrir el archivo.");
        return;
    }

    std::stringstream ss;
    ss << file.rdbuf();
    textbuf->text(ss.str().c_str());
    current_file_path = filepath;
}

///========= SAVE FILE =======///
void save_to_file(const std::string& path) {
    std::ofstream out(path);
    if (!out) {
        fl_alert("No se pudo guardar el archivo.");
        return;
    }
    out << textbuf->text();
    current_file_path = path;
}

///========== SAVE ==========///
void save_cb(Fl_Widget*, void*) {
    if (current_file_path.empty()) {
        const char* filepath = fl_file_chooser("Guardar como...", "*.txt", NULL);
        if (!filepath) return;
        save_to_file(filepath);
    } else {
        save_to_file(current_file_path);
    }
}

///========== SAVE ===========///
void save_as_cb(Fl_Widget*, void*) {
    const char* filepath = fl_file_chooser("Guardar como...", "*.txt", NULL);
    if (!filepath) return;
    save_to_file(filepath);
}

///====== SELECT THE ICON ====///
void set_icon(Fl_Window* win, const char* iconPath) {
    HWND hwnd = fl_xid(win);
    HICON hIcon = (HICON)LoadImageA(NULL, iconPath, IMAGE_ICON, 0, 0, LR_LOADFROMFILE | LR_DEFAULTSIZE);
    if (hIcon) {
        SendMessage(hwnd, WM_SETICON, ICON_BIG, (LPARAM)hIcon);
        SendMessage(hwnd, WM_SETICON, ICON_SMALL, (LPARAM)hIcon);
    }
}

///=========== MAIN =========///
int main() {
    Fl_Window* win = new Fl_Window(800, 600, "Simple Text Reader");

    Fl_Menu_Bar* menu = new Fl_Menu_Bar(0, 0, 800, 25);
    menu->add("File/New",     FL_CTRL + 'n', new_file_cb);
    menu->add("File/Open",    FL_CTRL + 'o', open_file_cb);
    menu->add("File/Save",    FL_CTRL + 's', save_cb);
    menu->add("File/Save As", FL_CTRL + FL_SHIFT + 's', save_as_cb);

    Fl_Text_Editor* editor = new Fl_Text_Editor(10, 35, 780, 550);
    textbuf = new Fl_Text_Buffer();
    editor->buffer(textbuf);

    win->end();
    win->show();
    set_icon(win, "Icon.ico");
    return Fl::run();
}
