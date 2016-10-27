#include <pebble.h>

#define CONFIG_TICKNESS_MARKS 2
#define CONFIG_CENTER_SECONDS_X 72
#define CONFIG_CENTER_SECONDS_Y 125
#define CONFIG_RADIUS_SECS_CIRCLE 25
#define CONFIG_HAND_LENGTH_SEC 22
#define CONFIG_HAND_LENGTH_SEC_INVERSE 10
#define CONFIG_HAND_LENGTH_HOUR 35
#define CONFIG_HAND_LENGTH_MIN 65
#define THICKNESS_MINUTES 2
#define THICKNESS_HOUR 2
#define THICKNESS_SECONDS 2

// Message sizes
const uint32_t inbox_size = 64;
const uint32_t outbox_size = 256;

typedef struct {
  int days;
  int hours;
  int minutes;
  int seconds;
} Time;





static Window *s_main_window;
static Layer *s_bg_layer,  *s_canvas_layer;
static BitmapLayer *s_background_layer;
static GBitmap *s_background_bitmap;

//static temp_vals[];

static Time s_last_time;
static char s_weekday_buffer[8], s_month_buffer[8], s_day_in_month_buffer[3];



static int32_t getMarkSize(int h){
  int32_t resultado = 75;
  switch(h){
    case 0  :
       resultado = 0;
       break; 
    case 1  :
       resultado = 65;
       break; 
    case 2  :
       resultado = 0;
       break;    
    case 3  :
       resultado = 64;
       break;    
    case 4  :
       resultado = 0;
       break;    
    case 5  :
       resultado = 64;
       break;    
    case 6  :
       resultado = 0;
       break;    
    case 7  :
       resultado = 64;
       break;    
    case 8  :
       resultado = 0;
       break;    
    case 9  :
       resultado = 65;
       break;    
    case 10  :
       resultado = 0 ;
       break;    
    case 11  :
       resultado = 65;
       break;       
} 
return resultado;
}


static int32_t getMarkLength(int h){
  int32_t resultado = 75;
  switch(h){
    case 0  :
       resultado = 0;
       break; 
    case 1  :
       resultado = 90;
       break; 
    case 2  :
       resultado = 0;
       break;    
    case 3  :
       resultado = 49;
       break;    
    case 4  :
       resultado = 0;
       break;    
    case 5  :
       resultado = 89;
       break;    
    case 6  :
       resultado = 0;
       break;    
    case 7  :
       resultado = 89;
       break;    
    case 8  :
       resultado = 0;
       break;    
    case 9  :
       resultado = 50;
       break;    
    case 10  :
       resultado = 0 ;
       break;    
    case 11  :
       resultado = 90;
       break;       
} 
return resultado;
}


int inverse_hand(int actual_time){
  int new_time = actual_time + 30;
  if (new_time > 60){
    new_time -= 60;
  }
  return new_time;
}  
/*
 * Este procedimiento devuelve un punto relativo al centro de la pantalla, para poder dibujar
 * una manecilla.
 *
 */
static GPoint make_hand_point(int quantity, int intervals, int len, GPoint center) {
  return (GPoint) {
    .x = (int16_t)(sin_lookup(TRIG_MAX_ANGLE * quantity / intervals) * (int32_t)len / TRIG_MAX_RATIO) + center.x,
    .y = (int16_t)(-cos_lookup(TRIG_MAX_ANGLE * quantity / intervals) * (int32_t)len / TRIG_MAX_RATIO) + center.y,
  };
}

/*
  Este procedimiento dibuja el fondo de la watchface
 */
static void bg_update_proc(Layer *layer, GContext *ctx) {
   GRect bounds = layer_get_bounds(layer);
  GPoint center = grect_center_point(&bounds);
  GPoint center_seconds = (GPoint) {
    .x = (int16_t)CONFIG_CENTER_SECONDS_X,
    .y = (int16_t)CONFIG_CENTER_SECONDS_Y,
  };
  //Minute markers
  
    graphics_context_set_stroke_color(ctx, GColorWhite);
    graphics_context_set_fill_color(ctx, GColorWhite);
  
 for(int h = 0; h < 60; h++) {   
        GPoint point = (GPoint) {
          .x = (int16_t)(sin_lookup(TRIG_MAX_ANGLE * h / 60) * (int32_t)(3 * CONFIG_HAND_LENGTH_MIN) / TRIG_MAX_RATIO) + center.x,
          .y = (int16_t)(-cos_lookup(TRIG_MAX_ANGLE * h / 60) * (int32_t)(3 * CONFIG_HAND_LENGTH_MIN) / TRIG_MAX_RATIO) + center.y,
        };
        graphics_draw_line(ctx, GPoint(center.x, center.y), GPoint(point.x, point.y));
    
  } 

 


   #if defined(PBL_COLOR)
     graphics_context_set_stroke_color(ctx, GColorOxfordBlue);
      graphics_context_set_fill_color(ctx, GColorOxfordBlue);
  #else
     graphics_context_set_stroke_color(ctx, GColorBlack);
      graphics_context_set_fill_color(ctx, GColorBlack);
  #endif   

  graphics_fill_rect(ctx, GRect(4, 4, bounds.size.w - (8), bounds.size.h - (8)), 0, GCornerNone);
 
    graphics_context_set_stroke_color(ctx, GColorWhite);
    graphics_context_set_fill_color(ctx, GColorWhite);
  
 //Hour markers
  for(int h = 0; h < 12; h++) { 
        GPoint point = (GPoint) {
          .x = (int16_t)(sin_lookup(TRIG_MAX_ANGLE * h / 12) * (int32_t)(getMarkLength(h)) / TRIG_MAX_RATIO) + center.x,
          .y = (int16_t)(-cos_lookup(TRIG_MAX_ANGLE * h / 12) * (int32_t)(getMarkLength(h)) / TRIG_MAX_RATIO) + center.y,
        };

        GPoint point02 = (GPoint) {
          .x = (int16_t)(sin_lookup(TRIG_MAX_ANGLE * h / 12) * getMarkSize(h) / TRIG_MAX_RATIO) + center.x,
          .y = (int16_t)(-cos_lookup(TRIG_MAX_ANGLE * h / 12) * getMarkSize(h) / TRIG_MAX_RATIO) + center.y,
        };
              
            for(int y = 0; y < CONFIG_TICKNESS_MARKS; y++) {
              for(int x = 0; x < CONFIG_TICKNESS_MARKS; x++) {
                graphics_draw_line(ctx, GPoint(point02.x + x, point02.y + y), GPoint(point.x + x, point.y + y));
              }
            }
    
  }


  // Aplite
 
    graphics_context_set_stroke_color(ctx, GColorWhite);
    graphics_context_set_fill_color(ctx, GColorWhite);
  
  // Seconds circle
  graphics_draw_circle(ctx,center_seconds,CONFIG_RADIUS_SECS_CIRCLE);
  // Marks  
  //graphics_fill_rect(ctx, GRect(70, 0, 4,10), 1, GCornersAll);  
  // Day window
  //graphics_fill_rect(ctx, GRect(CONFIG_X_START_INFO_BOX+10, 75, 25,22), 1, GCornersAll);  


  
  

}


static void refresh_temp_fore_graph(char fore[20]){

  APP_LOG(APP_LOG_LEVEL_DEBUG, "Val: %s", fore);

    //strtok(fore, "|");

  //while( token != NULL ) {
     // APP_LOG(APP_LOG_LEVEL_DEBUG, "Val: %d", (int)token);

  //  token = strtok(NULL, "|");
  //}
    

 //graphics_fill_rect(ctx, GRect(CONFIG_X_START_INFO_BOX+10, 75, 25,22), 1, GCornersAll);  

}

// static void update_forecast_graph(Layer *layer, GContext *ctx) {
//   // Custom drawing happens here
//   GRect bounds = layer_get_bounds(layer);
//   graphics_context_set_fill_color(ctx, GColorWhite);
//   for (int x = 0; x<5; x++){
//     graphics_fill_rect(ctx, GRect((x+(x*4+1)), 10, 4,12), 0, GCornersAll);  
//   }

// }

/*
  Este procedimiento dibuja las manecillas del reloj y todo lo que cambie cada TICK TIME

 */
static void draw_proc(Layer *layer, GContext *ctx) {

    GRect bounds = layer_get_bounds(layer);
    GPoint center = grect_center_point(&bounds);
    GPoint center_seconds = (GPoint) {
      .x =CONFIG_CENTER_SECONDS_X,
      .y = CONFIG_CENTER_SECONDS_Y,
    };



    Time now = s_last_time;

    GPoint second_hand_long = make_hand_point(now.seconds, 60, CONFIG_HAND_LENGTH_SEC, center_seconds);
    GPoint second_hand_inverse = make_hand_point(inverse_hand(now.seconds), 60, CONFIG_HAND_LENGTH_SEC_INVERSE, center_seconds);
    float minute_angle = TRIG_MAX_ANGLE * now.minutes / 60; //now.minutes
    float hour_angle = TRIG_MAX_ANGLE * now.hours / 12; //now.hours
    hour_angle += (minute_angle / TRIG_MAX_ANGLE) * (TRIG_MAX_ANGLE / 12);

  


    #if defined(PBL_COLOR)
     graphics_context_set_stroke_color(ctx, GColorRed);
      graphics_context_set_fill_color(ctx, GColorRed);
  #else
     graphics_context_set_stroke_color(ctx, GColorWhite);
      graphics_context_set_fill_color(ctx, GColorWhite);
  #endif   

      for(int y = 0; y < THICKNESS_SECONDS; y++) {
        for(int x = 0; x < THICKNESS_SECONDS; x++) {    
          graphics_draw_line(ctx, GPoint(center_seconds.x + x, center_seconds.y+y ), GPoint(second_hand_inverse.x + x, second_hand_inverse.y+y ));   
          graphics_draw_line(ctx, GPoint(center_seconds.x + x, center_seconds.y+y ), GPoint(second_hand_long.x + x, second_hand_long.y+y ));
        }
      }
     // Aplite
    
      graphics_context_set_stroke_color(ctx, GColorWhite);
      graphics_context_set_fill_color(ctx, GColorWhite);
    

    GPoint minute_hand_long = make_hand_point(now.minutes, 60, CONFIG_HAND_LENGTH_MIN, center);    
    GPoint hour_hand_long = (GPoint) {
      .x = (int16_t)(sin_lookup(hour_angle) * (int32_t)CONFIG_HAND_LENGTH_HOUR / TRIG_MAX_RATIO) + center.x,
      .y = (int16_t)(-cos_lookup(hour_angle) * (int32_t)CONFIG_HAND_LENGTH_HOUR / TRIG_MAX_RATIO) + center.y,
    };


    for(int y = 0; y < THICKNESS_MINUTES; y++) {
      for(int x = 0; x < THICKNESS_MINUTES; x++) {
        graphics_draw_line(ctx, GPoint(center.x + x, center.y + y), GPoint(minute_hand_long.x + x, minute_hand_long.y + y));
      }
    }
   

    for(int y = 0; y < THICKNESS_HOUR; y++) {
      for(int x = 0; x < THICKNESS_HOUR; x++) {
        graphics_draw_line(ctx, GPoint(center.x + x, center.y + y), GPoint(hour_hand_long.x + x, hour_hand_long.y + y));
      }
    }

  


  

  




 
    graphics_context_set_stroke_color(ctx, GColorBlack);
    graphics_context_set_fill_color(ctx, GColorWhite);
  
  
  // Hands center
  graphics_draw_circle(ctx, GPoint(center.x + 1, center.y + 1), 4);
  graphics_fill_circle(ctx, GPoint(center.x + 1, center.y + 1), 3);
  // Seconds center
  graphics_draw_circle(ctx, GPoint(center_seconds.x + 1, center_seconds.y + 1), 3);
  graphics_fill_circle(ctx, GPoint(center_seconds.x + 1, center_seconds.y + 1), 2);
 
    graphics_context_set_fill_color(ctx, GColorBlack);
 
  // Hands center
  graphics_fill_circle(ctx, GPoint(center.x + 1, center.y + 1), 1);
  // Seconds center
  graphics_fill_circle(ctx, GPoint(center_seconds.x + 1, center_seconds.y + 1), 1);  






}




static void tick_handler(struct tm *tick_time, TimeUnits changed) {
  
  s_last_time.days = tick_time->tm_mday;
  s_last_time.hours = tick_time->tm_hour;
  s_last_time.minutes = tick_time->tm_min;
  s_last_time.seconds = tick_time->tm_sec;
  snprintf(s_day_in_month_buffer, sizeof(s_day_in_month_buffer), "%d", s_last_time.days);
  strftime(s_weekday_buffer, sizeof(s_weekday_buffer), "%a", tick_time);
  strftime(s_month_buffer, sizeof(s_month_buffer), "%b", tick_time);  
  static char s_buffer[8];
  strftime(s_buffer, sizeof(s_buffer), clock_is_24h_style() ?"%H:%M" : "%I:%M", tick_time);
  layer_mark_dirty(s_canvas_layer);

  unsigned int now = mktime(tick_time);


}

static void window_load(Window *window) {
  Layer *window_layer = window_get_root_layer(window);
  GRect bounds = layer_get_bounds(window_layer);
  // Create Background Layer
  s_bg_layer = layer_create(bounds);
  layer_set_update_proc(s_bg_layer, bg_update_proc);
  layer_add_child(window_layer, s_bg_layer);

  s_background_layer = bitmap_layer_create(bounds);
  bitmap_layer_set_bitmap(s_background_layer, s_background_bitmap);
  bitmap_layer_set_compositing_mode(s_background_layer, GCompOpSet);

  s_canvas_layer = layer_create(bounds);
  layer_set_update_proc(s_canvas_layer, draw_proc);


  layer_add_child(window_layer, bitmap_layer_get_layer(s_background_layer));
  layer_add_child(window_layer, s_canvas_layer);
  
  
}


static void window_unload(Window *window) {
  bitmap_layer_destroy(s_background_layer);
  layer_destroy(s_bg_layer);
  layer_destroy(s_canvas_layer);
  gbitmap_destroy(s_background_bitmap);

 

}


static void save_config(){
 
 
}

static void load_config(){
 

}

static void init() {
  tick_timer_service_subscribe(SECOND_UNIT, tick_handler);
  s_main_window = window_create();
  s_background_bitmap = gbitmap_create_with_resource(RESOURCE_ID_BACKGROUND_BW_IMAGE);

  
   
   #if defined(PBL_COLOR)
     window_set_background_color(s_main_window, GColorOxfordBlue);
  #else
     window_set_background_color(s_main_window, GColorBlack);  
  #endif   
  


  window_set_window_handlers(s_main_window, (WindowHandlers) {
    .load = window_load,
    .unload = window_unload,
  });
  window_stack_push(s_main_window, true);

  time_t t = time(NULL);
  struct tm *tm_now = localtime(&t);
  tick_handler(tm_now, SECOND_UNIT);

  

   load_config();


   
}



static void deinit() {
   save_config();
   window_destroy(s_main_window);
}

int main(void) {
  init();
  app_event_loop();
  deinit();
}