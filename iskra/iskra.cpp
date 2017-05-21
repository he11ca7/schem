#include "iskra.h"

const int Iskra::_config[] =
{
    13, 12, 11, 10, 9, 8, 7, A0,
    0, 1, 2, 3, 4, 5, 6, A1
};

Errors Iskra::serialPost(
        byte command,
        byte operand)
{
    Errors code = ERROR_SERIAL_INTERNAL;

    byte buffer[2];
    buffer[0] = command;
    buffer[1] = operand;
    if(Serial.write(buffer, 2) == 2)
        code = ERROR_SUCCESS;
    Serial.flush();

    return code;
}

Iskra::Iskra()
{
    // Serial.readBytes() будет ждать данных 100 мс
    // Serial.setTimeout(100);

    // Инициализируем по умолчанию все пины как неиспользуемые
    for(int i = PINS_BEGIN; i <= PINS_END; ++i)
        _states[i] = STATE_UNUSED;
}

void Iskra::setState(
        Pins id,
        States state)
{
    _states[id] = state;
    switch (state)
        {
        case STATE_IN:
            pinMode(pin(id), OUTPUT);
            digitalWrite(pin(id), LOW);
            break;
        case STATE_OUT:
            pinMode(pin(id), INPUT_PULLUP);
            break;
        default: break;
        }
}

bool Iskra::setValue(
        Pins id,
        bool value)
{
    if(getState(id) == STATE_IN)
        {
            digitalWrite(pin(id), value);
            return true;
        }
    else
        return false;
}

bool Iskra::getValue(
        Pins id,
        bool &value)
{
    if(getState(id) == STATE_OUT)
        {
            value = digitalRead(pin(id));
            return true;
        }
    else
        return false;
}

Errors Iskra::serialGet(
        CommandsPC &command,
        Pins &pin)
{
    if (Serial.available() >= 2)
        {
            byte buffer[2];
            if(Serial.readBytes(buffer, 2) != 2)
                return ERROR_SERIAL_INTERNAL;

            if (
                    buffer[0] >= COMMANDSPC_BEGIN &&
                    buffer[0] <= COMMANDSPC_END)
                command = (CommandsPC) buffer[0];
            else
                return ERROR_UNKNOWN_COMMAND;

            if (
                    buffer[1] >= PINS_BEGIN &&
                    buffer[1] <= PINS_END)
                pin = (Pins) buffer[1];
            else
                return ERROR_UNKNOWN_PIN;

            if (Serial.available())
                // Данных больше, чем должно быть
                serialGet(command, pin);
        }
    else
        // Данных недостаточно
        return ERROR_SERIAL_DEFICIT;

    return ERROR_SUCCESS;
}

Errors Iskra::serialPost(
        CommandsDuino command,
        Pins pin)
{
    return serialPost((byte) command, (byte) pin);
}

Errors Iskra::serialPostError(
        Errors code)
{
    return serialPost((byte) COM_ERROR, (byte) code);
}

void Iskra::serialPostOutputs()
{
    delay(TIMEOUT);
    for(int i = PINS_BEGIN; i <= PINS_END; ++i)
        {
            Pins pin = (Pins) i;
            bool value;
            if(getValue(pin, value))
                Iskra::serialPost(
                            value ? COM_VAL_1 : COM_VAL_0,
                            pin);

            // DEBUG
            delay(TIMEOUT);
        }
}
