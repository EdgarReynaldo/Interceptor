

#include "P51TestCode.h"
#include "QuadRootsRevJ.h"

#include "src/Intercept.hpp"
#include "src/Object.hpp"

#include "allegro5/allegro.h"
#include "allegro5/allegro_font.h"
#include "allegro5/allegro_ttf.h"
#include "allegro5/allegro_primitives.h"

#include <cstdio>


const int sw = 1024;
const int sh = 768;
const double DT = 1.0/60.0;

int main(int argc , char** argv) {
   
   (void)argc;
   (void)argv;
   
   if (!al_init()) {return 1;}
   if (!al_init_primitives_addon()) {return 2;}
   if (!al_init_font_addon() || !al_init_ttf_addon()) {return 3;}
   
   if (!al_install_keyboard() || !al_install_mouse()) {return 10;}
   
   al_set_new_display_flags(ALLEGRO_WINDOWED | ALLEGRO_OPENGL);
   
   ALLEGRO_DISPLAY* d = al_create_display(sw , sh);
   
   ALLEGRO_EVENT_QUEUE* q = al_create_event_queue();
   
   ALLEGRO_TIMER* t = al_create_timer(1.0/60.0);
   
   ALLEGRO_FONT* f = al_load_ttf_font("Verdana.ttf" , -36 , 0);
   
   ALLEGRO_BITMAP* buf = al_create_bitmap(sw,sh);
   
   if (!d || !q || !t || !f || !buf) {
      printf("Setup incomplete.\n");
      return 1;
   }
   
   al_register_event_source(q , al_get_display_event_source(d));
   al_register_event_source(q , al_get_timer_event_source(t));
   al_register_event_source(q , al_get_keyboard_event_source());
   al_register_event_source(q , al_get_mouse_event_source());
   
   
   
   
   
   
   
   CObject o1(sw/2.0 , sh/2.0 , 50);
   CObject o2(sw/4.0 , sh/2.0 , 50);
   
   int msx = 0;
   int msy = 0;
   int ticks = 0;
   bool lmb = false;
   bool mmb = false;
   bool rmb = false;
   bool quit = false;
   bool redraw = true;
   bool clear = true;
   
   al_show_mouse_cursor(d);
   
   al_start_timer(t);
   
   while (!quit) {
      if (redraw) {
         al_set_target_bitmap(buf);
         if (clear) {
            al_clear_to_color(al_map_rgb(0,0,0));
         }
         
         o1.Draw(al_map_rgb(255,0,0));
         o2.Draw(al_map_rgb(255,255,0));

         double t = GetInterceptTime(o1 , o2);
         
         al_draw_textf(f , al_map_rgb(255,255,255) , sw/2.0 , 10.0 , ALLEGRO_ALIGN_CENTER , "DT = %6.3lf" , t);
         
         al_set_target_backbuffer(d);
         al_draw_bitmap(buf , 0 , 0 , 0);
         al_flip_display();
         redraw = false;
         ticks = 0;
      }
      do {
         ALLEGRO_EVENT ev;
         al_wait_for_event(q , &ev);
         if (ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
            quit = true;
         }
         if (ev.type == ALLEGRO_EVENT_KEY_DOWN && ev.keyboard.keycode == ALLEGRO_KEY_ESCAPE) {
            quit = true;
         }
         if (ev.type == ALLEGRO_EVENT_KEY_DOWN && ev.keyboard.keycode == ALLEGRO_KEY_R) {
            clear = !clear;
            redraw = true;
         }
         if (ev.type == ALLEGRO_EVENT_KEY_DOWN && ev.keyboard.keycode == ALLEGRO_KEY_S) {
            o2.mov.vel.Set(0,0);
            o2.mov.acc.Set(0,0);
            redraw = true;
         }
         if (ev.type == ALLEGRO_EVENT_TIMER) {
            redraw = true;
            ++ticks;
            if (ticks == 1) {
///               ctable.UpdateCollisionTableAndResolve(DT);
            }
         }
         if (ev.type == ALLEGRO_EVENT_MOUSE_AXES) {

            msx = ev.mouse.x;
            msy = ev.mouse.y;
//            msx = ev.mouse.x - sw/2.0;
//            msy = ev.mouse.y - sh/2.0;
            if (lmb) {
               o2.mov.pos.Set(msx,msy);
            }
            else if (rmb) {
               o2.mov.vel.Set(msx - o2.mov.pos.x , msy - o2.mov.pos.y);
            }
            else if (mmb) {
               o2.mov.acc.Set(msx - o2.mov.pos.x , msy - o2.mov.pos.y);
            }
            /// msx,msy holds the vector from the center of the sceen to the mouse pointer
         }
         if (ev.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN) {
            if (ev.mouse.button == 1) {
               lmb = true;
            }
            else if (ev.mouse.button == 2) {
               rmb = true;
            }
            else if (ev.mouse.button == 3) {
               mmb = true;
            }
         }
         if (ev.type == ALLEGRO_EVENT_MOUSE_BUTTON_UP) {
            if (ev.mouse.button == 1) {
               lmb = false;
            }
            else if (ev.mouse.button == 2) {
               rmb = false;
            }
            else if (ev.mouse.button == 3) {
               mmb = false;
            }
         }
      } while (!al_is_event_queue_empty(q));
   }
   
   return 0;
}


