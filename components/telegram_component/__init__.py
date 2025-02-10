import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import text_sensor
from esphome.const import CONF_ID

DEPENDENCIES = ['text_sensor']

telegram_component_ns = cg.esphome_ns.namespace('telegram_component')
TelegramComponent = telegram_component_ns.class_('TelegramComponent', cg.Component)

CONFIG_SCHEMA = cv.Schema({
    cv.GenerateID(): cv.declare_id(TelegramComponent),
    cv.Required(CONF_TOKEN): cv.string,
    cv.Required('text_sensor'): cv.use_id(text_sensor.TextSensor),
}).extend(cv.COMPONENT_SCHEMA)

def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID], config[CONF_TOKEN], config['text_sensor'])
    yield cg.register_component(var, config)
