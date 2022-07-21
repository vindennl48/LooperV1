#ifndef L_DMA_H
#define L_DMA_H

typedef struct LDMA {
  union {
    uint32_t data;
    struct {
      // Input to DMA channel.  Refer to the 127 options in the DMA MUX Mapping
      // table in the manual.
      unsigned source:7;

      unsigned ldma_reserved:22;

      // 0:disabled, 1:enabled
      /* Enables the DMA channel to be always on and does not need a reset in
       * between channel calls.  Normally, you will need to write to the
       * dma_enable bit to reset in-between calls. */
      unsigned always_enable:1;

      // 0:disabled, 1:enabled
      /* Enables the periodic trigger capabilities. */
      unsigned trigger_enable:1;

      // 0:disabled, 1:enabled
      /* Enables this DMA channel */
      unsigned dma_enable:1;
    };
  };
} LDMA_t[32];
#define DMA (*(LDMA_t *)IMXRT_DMA_ADDRESS)

#endif
