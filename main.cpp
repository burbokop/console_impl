
#include <iostream>
#include "painter.h"
#include <SDL2/SDL.h>
#include "png_reader.h"
#include <fstream>

using namespace std;


inline pixel_primitives::bitmap sdl_surface_to_bitmap(SDL_Surface* surf) {
    return pixel_primitives::bitmap {
        .matrix = reinterpret_cast<std::uint32_t*>(surf->pixels),
        .width = static_cast<std::size_t>(surf->w),
        .height = static_cast<std::size_t>(surf->h)
    };
}

int main()
{

    std::ifstream istr("/home/borys/Pictures/sculpture_64.png");
    if(!istr.is_open()) {
        throw std::runtime_error("file not open");
    }
    const auto img = png_reader::read(istr);

    //std::cout << "w: " << img.width << ", h:" << img.height << "\n";
    //exit(0);

    auto window = SDL_CreateWindow("app_using_spm", 0, 0, 400, 400, 0);

    auto sdl_surface = SDL_GetWindowSurface(window);

    cout << "Finished" << endl;

    painter p;

    surface s(true, 0xffffffff, false);

    std::size_t last_w = 0;
    std::size_t last_h = 0;

    e172::ElapsedTimer updateTimer(1000 / 30);
    while (true) {
        if(updateTimer.check()) {
            pixel_primitives::fill_area(s.bitmap(), 0, 0, s.bitmap().width, s.bitmap().height, 0x00000000);

            //p.paint(s.bitmap());

            //pixel_primitives::draw_rect(s.bitmap(), 0, 0, s.bitmap().width - 2, s.bitmap().height - 2, 0xff0000ff);

            //pixel_primitives::draw_grid(s.bitmap(), 10, 4, 60, 36, 4, 0xffff4800);

            //pixel_primitives::draw_circle(s.bitmap(), s.bitmap().width / 2, s.bitmap().height / 2, 12, 0xff00ff00);

            pixel_primitives::blit(s.bitmap(), img, 4, 4);

            if(s.bitmap().width != last_w || s.bitmap().height != last_h) {
                SDL_FreeSurface(sdl_surface);
                SDL_SetWindowSize(window, s.bitmap().width, s.bitmap().height);
                sdl_surface = SDL_GetWindowSurface(window);
                last_w = s.bitmap().width;
                last_h = s.bitmap().height;
            }
            SDL_FillRect(sdl_surface, nullptr, 0x00000000);
            SDL_LockSurface(sdl_surface);
            auto btmp = sdl_surface_to_bitmap(sdl_surface);
            pixel_primitives::copy(btmp, s.bitmap());
            SDL_UnlockSurface(sdl_surface);

            s.update();
            SDL_UpdateWindowSurface(window);
        }
    }


    return 0;
}
