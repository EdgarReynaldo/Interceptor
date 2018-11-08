


#include <cstdio>
#include <cmath>



#include "allegro5/allegro.h"
#include "allegro5/allegro_primitives.h"
#include "allegro5/allegro_font.h"
#include "allegro5/allegro_ttf.h"

#include "Circle.hpp"
#include "CollTable.hpp"
#include "Vector2.hpp"
#include "Arrow.hpp"

const int sw = 800;
const int sh = 600;
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
   
   if (!d || !q || !t) {
      printf("Setup incomplete.\n");
      return 1;
   }
   
   al_register_event_source(q , al_get_display_event_source(d));
   al_register_event_source(q , al_get_timer_event_source(t));
   al_register_event_source(q , al_get_keyboard_event_source());
   al_register_event_source(q , al_get_mouse_event_source());
   
   
   unsigned int NCMAX = 2000;/// This takes 1000*1999 collision pairs at max capacity
   unsigned int NC = 4;
   
   std::vector<Circle> circvec(NCMAX + 1 , Circle());
   
   double rad1 = sqrt(sw*sw*9.0/4.0 + sh*sh/4.0);
   double rad2 = sqrt(sw*sw/4.0 + sh*sh*9.0/4.0); 
   /// Circular boundaries
   circvec[0] = Circle(-3.0*sw/2.0 , sh/2.0 , rad1);///sqrt(sw*sw/4.0 + sh*sh/4.0));/// This is our main circle boundary
   circvec[1] = Circle(5.0*sw/2.0 , sh/2.0 , rad1);///sqrt(sw*sw/4.0 + sh*sh/4.0));/// This is our main circle boundary
   circvec[2] = Circle(sw/2.0 , -3.0*sh/2.0 , rad2);///sqrt(sw*sw/4.0 + sh*sh/4.0));/// This is our main circle boundary
   circvec[3] = Circle(sw/2.0 , 5.0*sh/2.0 , rad2);///sqrt(sw*sw/4.0 + sh*sh/4.0));/// This is our main circle boundary
   
   
   CollTable ctable;
   ctable.ReserveN(100);

   Circle* b1 = &circvec[0];
   Circle* b2 = &circvec[1];
   Circle* b3 = &circvec[2];
   Circle* b4 = &circvec[3];
   
   b1->fixed = true;
   b2->fixed = true;
   b3->fixed = true;
   b4->fixed = true;
   
   ctable.AddCircle(b1);
   ctable.AddCircle(b2);
   ctable.AddCircle(b3);
   ctable.AddCircle(b4);
   

   Vec2 p1(sw/2,sh/2);
   Vec2 p2(sw/2,sh/2);
   
   int msx = 0;
   int msy = 0;
   bool lmb = false;
   bool rmb = false;
   bool quit = false;
   bool redraw = true;
   
   al_show_mouse_cursor(d);
   
   al_start_timer(t);
   
   while (!quit) {
      if (redraw) {
         al_set_target_backbuffer(d);
         al_clear_to_color(al_map_rgb(0,0,0));
         
         /// Draw board here
         for (unsigned int i = 0 ; i < 4 ; ++i) {
            Circle* c = &circvec[i];
            c->Draw(al_map_rgb(255,255,255));
         }
         
         Vec2 c(sw/2,sh/2);
         DrawArrow(c , p1 , al_map_rgb(255,255,255));
         DrawArrow(c , p2 , al_map_rgb(0,255,0));
         DrawArrow(c , c + ScalarProjection(p1-c,p2-c) , al_map_rgb(255,0,0));

         for (unsigned int i = 4 ; i < NC ; ++i) {
            Circle* c = &circvec[i];
            if (c->active) {
               c->Draw(al_map_rgb(0,255,0));
            }
         }

         al_flip_display();
         redraw = false;
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
         if (ev.type == ALLEGRO_EVENT_TIMER) {
            if (lmb) {
            }
            ctable.UpdateCollisionTableAndResolve(DT);
            redraw = true;
         }
         if (ev.type == ALLEGRO_EVENT_MOUSE_AXES) {

            msx = ev.mouse.x;
            msy = ev.mouse.y;
//            msx = ev.mouse.x - sw/2.0;
//            msy = ev.mouse.y - sh/2.0;
            if (lmb) {
               p1.Set(msx,msy);
            }
            else if (rmb) {
               p2.Set(msx,msy);
            }
            /// msx,msy holds the vector from the center of the sceen to the mouse pointer
         }
         if (ev.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN) {
            if (ev.mouse.button == 1) {
               lmb = true;
               if (NC < NCMAX + 1) {
                  circvec[NC] = Circle(sw/2.0 , sh/2.0 , 25.0);
                  circvec[NC].SetSpeed(10*(msx - sw/2),10*(msy - sh/2));
                  ctable.AddCircle(&circvec[NC]);
                  ++NC;
               }
            }
            else if (ev.mouse.button == 2) {
               rmb = true;
            }
         }
         if (ev.type == ALLEGRO_EVENT_MOUSE_BUTTON_UP) {
            if (ev.mouse.button == 1) {
               lmb = false;
            }
            else if (ev.mouse.button == 2) {
               rmb = false;
            }
         }
         /*
            if (ev.mouse.button == 1) {
               if (NC < NCMAX + 1) {
                  circvec[NC] = Circle(sw/2.0 , sh/2.0 , 25.0);
                  circvec[NC].SetSpeed(msx,msy);
                  ctable.AddCircle(&circvec[NC]);
                  ++NC;
               }
            }
         */
            
      } while (!al_is_event_queue_empty(q));
   }
   
   
   return 0;
}
