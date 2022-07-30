#ifndef L_ADC_H
#define L_ADC_H

// Example calibration
// void setup() {
//   ADC1.calibration = 1;
//   while (ADC1.calibration) { delay(100); };
//   if ( ADC1.calibration_failed ) {
//     Serial.println("ADC -> ADC1 Calibration Failed");
//     delay(10000);
//   }
// }

// Example setup
// ADC1.clock_select          = 3; // ASYNC
// ADC1.conversion_mode       = 2; // 12bit
// ADC1.sample_mode           = 1; // long sample mode
// ADC1.clock_divide          = 1; // divide by 2
// ADC1.low_power             = 0; // no
// ADC1.sample_period         = 3; // 25 clock cycles
// ADC1.convert_speed         = 1; // high speed
// ADC1.voltage_reference     = 0; // only option
// ADC1.trigger_source        = 0; // software
// ADC1.sample_average        = 3; // 32 samples
// ADC1.data_overwrite        = 1; // yes
// ADC1.continuous_conversion = 1; // yes
// ADC1.DMA_enable            = 0; // no
// ADC1.adc_set[0].channel    = 8; // Hardware Pin 15, this should be last

/* ADC1.adc_set[x].channel should be called last if you are using continuous
 * conversion.  If it is not called last, you will have to re-call it later to
 * activate the ADC in software mode.  If it is running in hardware mode, this
 * shouldn't matter.
 *
 * Continuous mode should be used if you are only using one analog input.  If
 * you are using multiple analog inputs, you will have to switch between
 * channels.  When switching channels, the software trigger will be initiated
 * anyway so continuous mode isn't needed. */


typedef struct ADC {
  /* ADCx_HCn */
  union {
    volatile uint32_t adc_set;
    struct {
      /* input channel select for ADC. Make sure to use 'Channel' column below
       * List of avaliable channels:
       * Hardware Pin | Analog Pin |  ADC  | Channel
       *      14      |      0     |   1   |    7   
       *      15      |      1     |   1   |    8   
       *      16      |      2     |   1   |    12  
       *      17      |      3     |   1   |    11  
       *      18      |      4     |   1   |    6   
       *      19      |      5     |   1   |    5   
       *      20      |      6     |   1   |    15  
       *      21      |      7     |1 and 2|    0   
       *      22      |      8     |   1   |    13  
       *      23      |      9     |   1   |    14  
       *      24      |      10    |   1   |    1   
       *      25      |      11    |   1   |    2   
       *      26      |      12    |   2   |    3   
       *      27      |      13    |   2   |    4   
       *      38      |      14    |   2   |    1   
       *      39      |      15    |   2   |    2   
       *      40      |      16    |   1   |    9   
       *      41      |      17    |   1   |    10  */
      volatile unsigned channel:5;

      volatile unsigned adc_hc_reserved:2;

      // 0:no interrupt after conversion, 1:interrupt after every conversion
      volatile unsigned conversion_complete_interrupt:1;

      volatile unsigned adc_hc_reserved2:24;
    };
  } adc_set[8];

  union {
    volatile uint32_t adc_hs_status;
    struct {
      /* The COCOn flag is a read-only bit that is set each time a conversion
       * is completed when the compare function is disabled (ADC_GC[ACFE]=0)
       * and the hardware average function is disabled (ADC_GC[AVGE]=0). When
       * the compare function is enabled (ADC_GC[ACFE]=1), the COCOn flag is set
       * upon completion of a conversion only if the compare result is true.
       * When the hardware average function is enabled (ADC_GC[AVGE]=1), the
       * COCOn flag is set upon completion of the selected number of
       * conversions (determined by the ADC_CFG[AVGS] field). The COCO0 flag
       * will also set at the completion of a Calibration and Test sequence. A
       * COCOn bit is cleared when the respective ADC_HCn is written or when
       * the respective ADC_Rn is read.
       * NOTE:
       *   - In the hardware trigger mode, when trigger comes from TSC, COCO
       *     bit can be cleared by writing TSC register. When other ADC_ETC
       *     trigger sources work as ADC hardware trigger, COCO bit is cleared
       *     automatically when each phase is done.
       *   - In the software trigger mode, COCO0 bit is cleared when ADC_HC0
       *     is written or when ADC_R0 is read.*/
      volatile uint32_t conversion_complete:1;

      volatile uint32_t adc_hs_status_reserved:31;
    };
  };

  /* ADCx_Rn */
  union {
    volatile uint32_t adc_read;
    struct {
      // READONLY, result from the ADC after conversion
      volatile unsigned result:12;

      volatile unsigned adc_read_reserved:20;
    };
  } adc_read[8];

  /* ADCx_CFG */
  union {
    volatile uint32_t adc_config;
    struct {
      // 0:IPG, 1:IPG/2, 2:reserved, 3:ASYNC clock
      volatile unsigned clock_select:2;

      // 0:8bit, 1:10bit, 2:12bit, 3:reserved
      volatile unsigned conversion_mode:2;

      /* 0:short sample mode, 1:long sample mode. works hand-in-hand with
        'sample_period' below. */
      volatile unsigned sample_mode:1;

      // 0:none, 1:divide by 2, 2:divide by 4, 3:divide by 8
      volatile unsigned clock_divide:2;

      // 0:no, 1:yes
      volatile unsigned low_power:1;

      /* Has a couple modes depending on 'sample_mode' above.  This allows longer
      * sampling timeframes for higher impedance vs low impedance inputs.
      * Below will show sample_period & sample_mode:
      * - 0 & 0: 3 clock cycles
      * - 0 & 1: 13 clock cycles
      * - 1 & 0: 5 clock cycles
      * - 1 & 1: 17 clock cycles
      * - 2 & 0: 7 clock cycles
      * - 2 & 1: 21 clock cycles
      * - 3 & 0: 9 clock cycles
      * - 3 & 1: 25 clock cycles */
      volatile unsigned sample_period:2;

      // 0:normal-speed, 1:high-speed
      volatile unsigned convert_speed:1;

      //  Only option available is 0, all other options are reserved
      volatile unsigned voltage_reference:2;

      // 0:software trigger, 1:hardware trigger
      // Software trigger is triggered by a write to ADCn_HC0
      // Hardware trigger is triggered by a hardware pin
      volatile unsigned trigger_source:1;

      // 0:4 samples, 1:8 samples, 2:16 samples, 3:32 samples
      volatile unsigned sample_average:2;

      // 0:no, 1:yes
      /* Overwrite the data in the output register if the data has not been read by
      * the CPU yet */
      volatile unsigned data_overwrite:1;

      volatile unsigned adc_config_reserved:15;
    };
  };

  /* ADCx_GC */
  union {
    volatile uint32_t adc_general_control;
    struct {
      // 0:not forced, async clock only active if selected by ADCn_CONFIG
      // 1:forced, asyc clock is enabled regardless of ADCn_CONFIG state
      volatile unsigned async_clock_force:1;

      // 0:DMA buffers not enabled, 1:DMA buffers enabled
      volatile unsigned DMA_enable:1;

      // 0:range function disabled, 1:range function enabled
      // Used for comparitor functions similar to an opamp
      volatile unsigned compare_range:1;

      // 0:less than threshold, 1:greater than threshold
      // Used for comparitor functions similar to an opamp
      volatile unsigned compare_greater_than:1;

      // 0:disabled, 1:enabled
      // Used for comparitor functions similar to an opamp
      volatile unsigned compare_enable:1;

      // 0:disabled, 1:enabled
      // enables hardware averaging function of the ADC
      volatile unsigned hardware_average:1;

      // 0:disabled, 1:enabled
      // As soon as the previous conversion is complete, immediately start the
      // next conversion without software or hardware triggers
      volatile unsigned continuous_conversion:1;

      // 1:run calibration
      // Conversion will run until complete.  Once complete, this flag will
      // reset to 0 on it's own.  Details of the calibration are sent to
      // ADC_GS[CALF].
      volatile unsigned calibration:1;

      volatile unsigned adc_general_control_reserved:24;
    };
  };

  /* ADCx_GS */
  /* These are for getting status information from the ADC */
  union {
    volatile uint32_t adc_general_status;
    struct {
      // 0:conversion not in progress, 1:conversion is in progress
      volatile unsigned conversion_active:1;

      // 0:calibration completed normally, 1:calibration failed
      volatile unsigned calibration_failed:1;

      // 1:async wakeup interrupt occurred in stop mode, 0:no async interrupt
      /* Holds the status of asynchronous interrupt status that occurred during
       * stop mode. This bit is set when ipg_stop is deasserted and ipg_clk has
       * started. It is cleared by writing ‘1’ to it. Clearing this bit also
       * deasserts the Asynchronous interrupt to CPU. */
      volatile unsigned async_wakeup_interrupt_status:1;

      volatile unsigned adc_general_status_reserved:29;
    };
  };


  /* ADCx_CV */
  /* Not used in my case.  This is to set values to compare with ADC */
  volatile uint32_t adc_compare_value;

  /* ADCx_OFS */
  /* Not used in my case.  This is to correct a slight offset with ADC input */
  volatile uint32_t adc_offset_correction_value;

  /* ADCx_CAL */
  /* READONLY, when calibration is run, this is the number the ADC uses. */
  volatile uint32_t adc_calibration_value;

} ADC_t;
#define ADC1 (*(ADC_t *)IMXRT_ADC1_ADDRESS)
#define ADC2 (*(ADC_t *)IMXRT_ADC2_ADDRESS)

#endif
