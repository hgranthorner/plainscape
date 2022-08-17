add_library(Imgui
        imconfig.h
        imgui.cpp imgui.h
        imgui_draw.cpp
        imgui_internal.h
        imgui_tables.cpp
        imgui_widgets.cpp
        imstb_rectpack.h
        imstb_textedit.h
        imstb_truetype.h
        backends/imgui_impl_glfw.cpp backends/imgui_impl_glfw.h
        backends/imgui_impl_opengl3.cpp backends/imgui_impl_opengl3.h)
target_include_directories(Imgui PUBLIC ./)
