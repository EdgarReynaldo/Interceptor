

#include "Merlin/QuadRootsRevJ.h"

#include "Merlin.hpp"

#include "allegro5/allegro.h"
#include "allegro5/allegro_font.h"
#include "allegro5/allegro_ttf.h"
#include "allegro5/allegro_primitives.h"

#include <cstdio>


const int sw = 1024;
const int sh = 768;
const double DT = 1.0/60.0;

///ALLEGRO_FONT* f = 0;

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
   
   f = al_load_ttf_font("Verdana.ttf" , -36 , 0);
   
   ALLEGRO_BITMAP* buf = al_create_bitmap(sw,sh);
   
   if (!d || !q || !t || !f || !buf) {
      printf("Setup incomplete.\n");
      return 1;
   }
   
   al_register_event_source(q , al_get_display_event_source(d));
   al_register_event_source(q , al_get_timer_event_source(t));
   al_register_event_source(q , al_get_keyboard_event_source());
   al_register_event_source(q , al_get_mouse_event_source());
   

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
   circvec[4].SetAccel(0,100);
   
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
   bool pause = true;
   
   al_show_mouse_cursor(d);
   
   al_start_timer(t);
   
   while (!quit) {
      if (redraw) {
         al_set_target_bitmap(buf);
         if (clear) {
            al_clear_to_color(al_map_rgb(0,0,0));
         }
         
         double tfirst = 1000;
         CObject* o = 0;
         for (unsigned int i = 0 ; i < 4 ; ++i) {
            o = &circvec[i];
            o->Draw(al_map_rgb(255,255,255));
         }
         for (unsigned int i = 0 ; i < 4 ; ++i) {
            o = &circvec[i];
            double t = GetInterceptTime(*o , circvec[4]);
         }
         o = &circvec[4];
         o->Draw(al_map_rgb(0,255,0));
         
         const double ddt = 0.25;
         double dt1;
         for (dt1 = -5.0 ; dt1 < 5.0 ; dt1 += ddt) {
            double dt2 = dt1 + ddt;
            MoveInfo i1 = o->FutureInfo(dt1);
            MoveInfo i2 = o->FutureInfo(dt2);
            DrawArrow(i1.pos , i2.pos , al_map_rgb(0,255,255));
         }

///         al_draw_textf(f , al_map_rgb(255,255,255) , sw/2.0 , 10.0 , ALLEGRO_ALIGN_CENTER , "DT = %6.3lf" , t);
         
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
         if (ev.type == ALLEGRO_EVENT_KEY_DOWN && ev.keyboard.keycode == ALLEGRO_KEY_P) {
            pause = !pause;
            redraw = true;
         }
         if (ev.type == ALLEGRO_EVENT_KEY_DOWN && ev.keyboard.keycode == ALLEGRO_KEY_S) {
            c1->SetSpeed(0,0);
            c1->SetAccel(0,0);
            redraw = true;
         }
         if (ev.type == ALLEGRO_EVENT_TIMER) {
            redraw = true;
            ++ticks;
            if (ticks == 1) {
               if (!pause) {
                  ctable.MarkDirty();
                  ctable.UpdateCollisionTableAndResolve(DT);
               }
            }
         }
         if (ev.type == ALLEGRO_EVENT_MOUSE_AXES) {

            msx = ev.mouse.x;
            msy = ev.mouse.y;
//            msx = ev.mouse.x - sw/2.0;
//            msy = ev.mouse.y - sh/2.0;
            if (lmb) {
               c1->SetPos(msx,msy);
            }
            else if (rmb) {
               c1->SetSpeed(msx - c1->Pos().x , msy - c1->Pos().y);
            }
            else if (mmb) {
               c1->SetAccel(msx - c1->Pos().x , msy - c1->Pos().y);
            }
            /// msx,msy holds the vector from the center of the sceen to the mouse pointer
         }
         if (ev.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN) {
            if (ev.mouse.button == 1) {
               lmb = true;
               c1->SetPos(msx,msy);
            }
            else if (ev.mouse.button == 2) {
               c1->SetSpeed(msx - c1->Pos().x , msy - c1->Pos().y);
               rmb = true;
            }
            else if (ev.mouse.button == 3) {
               c1->SetAccel(msx - c1->Pos().x , msy - c1->Pos().y);
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


