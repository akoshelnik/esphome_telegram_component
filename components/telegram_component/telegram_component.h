#pragma once

#include "esphome.h"
#include "esp_http_client.h"
// #include "cJSON.h"

class TelegramComponent : public Component {
 public:
  TelegramComponent(std::string token, TextSensor *text_sensor) : token_(token), text_sensor_(text_sensor) {}

  void setup() override {
    // Nothing to do here
  }

  void loop() override {
    // Nothing to do here
  }

  void make_request() {
    std::string response;
    esp_http_client_config_t config = {
      .url = ("https://api.telegram.org/bot" + token_ + "/getUpdates").c_str(),
      .event_handler = _http_event_handler,
      .user_data = &response,
    };

    esp_http_client_handle_t client = esp_http_client_init(&config);
    esp_http_client_set_method(client, HTTP_METHOD_POST);
    esp_http_client_set_post_field(client, "{}", strlen("{}"));

    esp_err_t err = esp_http_client_perform(client);
    if (err == ESP_OK) {
      ESP_LOGD("telegram", "HTTP POST Status = %d, content_length = %d",
               esp_http_client_get_status_code(client),
               esp_http_client_get_content_length(client));
      ESP_LOGD("telegram", "Response: %s", response.c_str());

     /*
      cJSON *json = cJSON_Parse(response.c_str());
      if (!json) {
        ESP_LOGE("telegram", "Failed to parse JSON response");
      } else {
        cJSON *result = cJSON_GetObjectItem(json, "result");
        if (!result) {
          ESP_LOGE("telegram", "No result key in JSON response");
        } else {
          cJSON *item;
          cJSON_ArrayForEach(item, result) {
            cJSON *message = cJSON_GetObjectItem(item, "message");
            if (message) {
              cJSON *text = cJSON_GetObjectItem(message, "text");
              if (text) {
                ESP_LOGD("telegram", "Received message: %s", text->valuestring);
                text_sensor_->publish_state(text->valuestring);
              }
            }
          }
        }
        cJSON_Delete(json);
      }
     */
    } else {
      ESP_LOGE("telegram", "HTTP POST request failed: %s", esp_err_to_name(err));
    }

    esp_http_client_cleanup(client);
  }

 protected:
  std::string token_;
  TextSensor *text_sensor_;

  static esp_err_t _http_event_handler(esp_http_client_event_t *evt) {
    switch (evt->event_id) {
      case HTTP_EVENT_ERROR:
        ESP_LOGE("telegram", "HTTP_EVENT_ERROR");
        break;
      case HTTP_EVENT_ON_DATA:
        if (evt->data_len > 0) {
          std::string *response = (std::string *)evt->user_data;
          response->append((char *)evt->data, evt->data_len);
        }
        break;
      default:
        break;
    }
    return ESP_OK;
  }
};
