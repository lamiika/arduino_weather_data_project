/******************************************************************************
 * File: ./WindSpeed.h
 * Requires: Utils.h
 ******************************************************************************/
namespace WindSpeed
{
    constexpr struct { uint8_t input = 2; } pin;
    constexpr uint8_t t_size = 10;
    volatile unsigned long t_begin = 0, t_end = 0;
    volatile unsigned long t_array[t_size] = {};

    volatile float max = 0, mean = 0, min = 0;
    volatile bool flag = 0;

    void InterruptServiceRoutine(void)
    {
        static volatile char i = 0;

        t_begin = t_end;
        t_end = millis();

        i = i < t_size ? i : 0;

        t_array[i++] = t_end - t_begin;

        flag = true;
    }

    void Update(void)
    {
        float sum = 0, t_max = 0 , t_min = 1000, tmp = 0;

        for(int i=0 ; i < t_size ; i++)
        {
           sum += tmp = millis2hz(t_array[i]) * 0.699 ;
           sum -= sum > 0 ? 0.24 : 0 ;

           t_max = t_max < tmp ? tmp : t_max;
           t_min = t_min > tmp ? tmp : t_min;
        }

        mean =  flag ? sum / t_size : 0;
        max =   flag ? t_max : 0;
        min =   flag ? t_min : 0;

        flag = false;

    }

    inline void Init(void)
    {
        pinMode(pin.input,INPUT);
        attachInterrupt(
            digitalPinToInterrupt(pin.input),
            InterruptServiceRoutine,
            RISING
        );
    }
}
