#pragma once

#include "lib_led.h"

class Effect
{
    uint8_t fps = 60;
    
public:
    Effect() { }
    /* Инициализация режима, установка начальный значений.
    *  Выполняется единожды, при выборе нового режима.
    * */
    
    virtual void on_init() {out("eff\n");}

    /* on_update обязательно нужно описать в наследукмом классе.
    *  Обновление логики эффекта.
    *  Часть цикла работы режима.
    * */
    virtual void on_update() = 0;

    /* on_render может быть переинициализирован в наследукмом классе.
    *  Обновление графики эффекта.
    *  Часть цикла работы режима.
    * */
    virtual void on_render() {}

    /* on_clear может быть переинициализирован в наследукмом классе.
     * Выполняется единожды, при выборе нового режима, перед вызовом on_init.
    *  Действия с состоянием матрицы оставшейся после предыдущего режима.
    *  По умолчанию всё стрирается.
    * */
    virtual void on_clear() {
        FastLED.clear();
    }

    /* get_fps устанавливает максимальный фпс для режима. Может регулировать скорость режима, 
     * однако скорость режима лучше регулировать кодов внутри режима.
    * */
    void set_fps(uint8_t val){
        fps = val;
    }

    /* get_fps возвращает максимально допустимый фпс для режима. ФПС может регулировать скорость режима, 
    * однако скорость режима лучше регулировать кодов внутри режима.
    *  По умолчанию 60.
    * */
    uint8_t get_fps(){
        return fps;
    }
};
