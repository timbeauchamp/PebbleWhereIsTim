#include <pebble.h>

static Window *sMyWindow;
static TextLayer *sTextLayer;

static void main_window_load(Window *window) 
{
    sTextLayer = text_layer_create(GRect(0, 55, 144, 50));
    text_layer_set_background_color(sTextLayer, GColorClear);
    text_layer_set_text_color(sTextLayer, GColorBlack);
//    text_layer_set_text(sTextLayer, "00:00");
    
    text_layer_set_font(sTextLayer, fonts_get_system_font(FONT_KEY_BITHAM_42_BOLD));
    text_layer_set_text_alignment(sTextLayer, GTextAlignmentCenter);

    layer_add_child(window_get_root_layer(window), text_layer_get_layer(sTextLayer));

}

static void main_window_unload(Window *window) 
{
    text_layer_destroy(sTextLayer);
}

static void update_time() 
{
  // Get a tm structure
  time_t temp = time(NULL); 
  struct tm *tick_time = localtime(&temp);

  // Create a long-lived buffer
  static char buffer[] = "00:00";

  // Write the current hours and minutes into the buffer
  if(clock_is_24h_style() == true) 
  {
    // Use 24 hour format
    strftime(buffer, sizeof("00:00"), "%H:%M", tick_time);
  } else {
    // Use 12 hour format
    strftime(buffer, sizeof("00:00"), "%I:%M", tick_time);
  }

  // Display this time on the TextLayer
  text_layer_set_text(sTextLayer, buffer);
}

static void tick_handler(struct tm *tick_time, TimeUnits units_changed) 
{
    update_time();
}

void handle_init(void) 
{
    sMyWindow = window_create();
    
    window_set_window_handlers(sMyWindow, (WindowHandlers) 
    {
        .load = main_window_load,
        .unload = main_window_unload
    });

    window_stack_push(sMyWindow, true);
    update_time();
    tick_timer_service_subscribe(MINUTE_UNIT, tick_handler);
}


void handle_deinit(void) 
{
    text_layer_destroy(sTextLayer);
    window_destroy(sMyWindow);
}

int main(void) 
{
    handle_init();
    app_event_loop();
    handle_deinit();
}
