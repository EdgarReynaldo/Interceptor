


#include <cstdio>
#include <cmath>



#include "allegro5/allegro.h"
#include "allegro5/allegro_primitives.h"
#include "allegro5/allegro_font.h"
#include "allegro5/allegro_ttf.h"

#include "Object.hpp"
#include "CollTable.hpp"
#include "Vector2.hpp"
#include "Arrow.hpp"
#include "Triangles.hpp"
#include "Overlap.hpp"
#include "Intercept.hpp"



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
   
   
   
   
   
   
   
   unsigned int NCMAX = 2000;/// This takes 1000*1999 collision pairs at max capacity
   unsigned int NC = 5;
   
   std::vector<CObject> circvec(NCMAX + 1 , CObject());
   
   double rad1 = sqrt(sw*sw*9.0/4.0 + sh*sh/4.0);
   double rad2 = sqrt(sw*sw/4.0 + sh*sh*9.0/4.0); 
   /// Circular boundaries
   circvec[0] = CObject(-3.0*sw/2.0 , sh/2.0 , rad1);///sqrt(sw*sw/4.0 + sh*sh/4.0));/// This is our main circle boundary
   circvec[1] = CObject(5.0*sw/2.0 , sh/2.0 , rad1);///sqrt(sw*sw/4.0 + sh*sh/4.0));/// This is our main circle boundary
   circvec[2] = CObject(sw/2.0 , -3.0*sh/2.0 , rad2);///sqrt(sw*sw/4.0 + sh*sh/4.0));/// This is our main circle boundary
   circvec[3] = CObject(sw/2.0 , 5.0*sh/2.0 , rad2);///sqrt(sw*sw/4.0 + sh*sh/4.0));/// This is our main circle boundary
   circvec[4] = CObject(sw/2.0 - 5 , sh/2.0 , 60);///sqrt(sw*sw/4.0 + sh*sh/4.0));/// This is our main circle boundary
   circvec[4].SetAccel(0,20);
   
   CollTable ctable;
   ctable.ReserveN(100);

   CObject* b1 = &circvec[0];
   CObject* b2 = &circvec[1];
   CObject* b3 = &circvec[2];
   CObject* b4 = &circvec[3];
   CObject* c1 = &circvec[4];
   
   b1->fixed = true;
   b2->fixed = true;
   b3->fixed = true;
   b4->fixed = true;
   
   ctable.AddObject(b1);
   ctable.AddObject(b2);
   ctable.AddObject(b3);
   ctable.AddObject(b4);
   ctable.AddObject(c1);
   
   
   Triangle tri1(Vec2(200 , 250) , Vec2(824 , 250) , Vec2(512 , 500));

   Vec2 p1(sw/2,sh/2);
   Vec2 p2(sw/2,sh/2);
   
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
         
         double t = GetInterceptTime(o1 , o2);
         
         o1.DrawHollow(al_map_rgb(255,255,255));
         o2.DrawHollow(al_map_rgb(0,255,0));
         
         DrawArrow(o2.mov.pos , o2.mov.pos + o2.mov.vel , al_map_rgb(0,0,255));
         
         al_draw_textf(f , al_map_rgb(255,255,255) , sw/2.0 , 10.0 , ALLEGRO_ALIGN_CENTER , "DT = %6.3lf" , t);
         
/*
         
         /// Draw board here
         int ec = 0;
         for (unsigned int i = 0 ; i < 4 ; ++i) {
            CObject* c = &circvec[i];
            c->DrawHollow(al_map_rgb(255,255,255));
         }
         
         for (unsigned int j = 4 ; j < NC ; ++j) {
            CObject* c = &circvec[j];
            if (c->mov.pos.x < 0.0 || c->mov.pos.x > sw || c->mov.pos.y < 0.0 || c->mov.pos.y > sh) {
               ++ec;
            }
         }
         Vec2 c(sw/2,sh/2);
         DrawArrow(c , p1 , al_map_rgb(255,255,255));
         DrawArrow(c , p2 , al_map_rgb(0,255,0));
         DrawArrow(c , c + ScalarProjection(p1-c,p2-c) , al_map_rgb(255,0,0));
         for (unsigned int i = 4 ; i < NC ; ++i) {
            CObject* c = &circvec[i];
            if (c->active) {
               c->Draw(al_map_rgb(0,255,0));
               al_draw_textf(f , al_map_rgb(0,255,0) , c->mov.pos.x , c->mov.pos.y - al_get_font_line_height(f)/2 , ALLEGRO_ALIGN_CENTRE , "%u" , i);
            }
            for (unsigned int j = 0 ; j < NC ; ++j) {
               if (i == j) {continue;}
               CObject* c2 = &circvec[j];
               if (c && c2 && Overlaps(*c , *c2)) {
                  c->Draw(al_map_rgb(255,0,0));
                  c2->Draw(al_map_rgb(255,0,0));
                  al_draw_textf(f , al_map_rgb(0,0,255) , c->mov.pos.x , c->mov.pos.y - al_get_font_line_height(f)/2 , ALLEGRO_ALIGN_CENTRE , "%u" , i);
               }
            }
         }
*/
         
///         tri1.Draw(al_map_rgb(0 , 0 , 0) , al_map_rgb(255,255,255) , 3.0);
         
///         TRISIDE side = GetTriSide(Vec2(msx,msy) , tri1);
         
///         const char* text = (side == TRI_INSIDE)?"Inside":(side == TRI_OUTSIDE)?"Outside":"On Edge";
         
///         al_draw_textf(f , al_map_rgb(255,255,255) , sw/2 , sh/2 - al_get_font_line_height(f)/2 , ALLEGRO_ALIGN_CENTER , "%s" , text);
         
///         al_draw_textf(f , al_map_rgb(0,255,255) , sw/2 , 10 , ALLEGRO_ALIGN_CENTER , "Escape count %d" , ec);

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
/*
               if (NC < NCMAX + 1) {
                  circvec[NC] = CObject(sw/2.0 , sh/2.0 , 15.0);
                  circvec[NC].SetSpeed(10*(msx - sw/2),10*(msy - sh/2));
                  circvec[NC].SetAccel(0.0 , 10.0);
                  ctable.AddObject(&circvec[NC]);
                  ++NC;
               }
*/
            }
            else if (ev.mouse.button == 2) {
               rmb = true;
/*
               if (NC < NCMAX + 1) {
                  circvec[NC] = CObject(sw/2.0 , sh/2.0 , 40.0);
                  circvec[NC].SetSpeed(10*(msx - sw/2),10*(msy - sh/2));
                  circvec[NC].SetAccel(0.0 , 10.0);
                  ctable.AddObject(&circvec[NC]);
                  ++NC;
               }
*/
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
