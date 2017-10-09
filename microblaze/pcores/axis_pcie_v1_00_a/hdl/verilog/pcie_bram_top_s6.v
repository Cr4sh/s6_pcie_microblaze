`timescale 1ns/1ns

module pcie_bram_top_s6 #(
  parameter DEV_CAP_MAX_PAYLOAD_SUPPORTED = 0,

  parameter VC0_TX_LASTPACKET         = 31,
  parameter TLM_TX_OVERHEAD           = 20,
  parameter TL_TX_RAM_RADDR_LATENCY   = 1,
  parameter TL_TX_RAM_RDATA_LATENCY   = 2,
  parameter TL_TX_RAM_WRITE_LATENCY   = 1,

  parameter VC0_RX_LIMIT              = 'h1FFF,
  parameter TL_RX_RAM_RADDR_LATENCY   = 1,
  parameter TL_RX_RAM_RDATA_LATENCY   = 2,
  parameter TL_RX_RAM_WRITE_LATENCY   = 1
) (
  input          user_clk_i,
  input          reset_i,

  input          mim_tx_wen,
  input  [11:0]  mim_tx_waddr,
  input  [35:0]  mim_tx_wdata,
  input          mim_tx_ren,
  input          mim_tx_rce,
  input  [11:0]  mim_tx_raddr,
  output [35:0]  mim_tx_rdata,

  input          mim_rx_wen,
  input  [11:0]  mim_rx_waddr,
  input  [35:0]  mim_rx_wdata,
  input          mim_rx_ren,
  input          mim_rx_rce,
  input  [11:0]  mim_rx_raddr,
  output [35:0]  mim_rx_rdata
);

  // TX calculations
  localparam MPS_BYTES = ((DEV_CAP_MAX_PAYLOAD_SUPPORTED == 0) ? 128 :
                          (DEV_CAP_MAX_PAYLOAD_SUPPORTED == 1) ? 256 :
                                                                 512 );

  localparam BYTES_TX = (VC0_TX_LASTPACKET + 1) * (MPS_BYTES + TLM_TX_OVERHEAD);

  localparam ROWS_TX = 1;
  localparam COLS_TX = ((BYTES_TX <= 2048) ? 1 :
                        (BYTES_TX <= 4096) ? 2 :
                        (BYTES_TX <= 8192) ? 4 :
                                             9
                       );

  // RX calculations
  localparam ROWS_RX = 1;

  localparam COLS_RX = ((VC0_RX_LIMIT < 'h0200) ? 1 :
                        (VC0_RX_LIMIT < 'h0400) ? 2 :
                        (VC0_RX_LIMIT < 'h0800) ? 4 :
                                                  9
                       );

  pcie_brams_s6 #(
    .NUM_BRAMS        (COLS_TX),
    .RAM_RADDR_LATENCY(TL_TX_RAM_RADDR_LATENCY),
    .RAM_RDATA_LATENCY(TL_TX_RAM_RDATA_LATENCY),
    .RAM_WRITE_LATENCY(TL_TX_RAM_WRITE_LATENCY))
  pcie_brams_tx
  (
    .user_clk_i(user_clk_i),
    .reset_i(reset_i),

    .waddr(mim_tx_waddr),
    .wen(mim_tx_wen),
    .ren(mim_tx_ren),
    .rce(mim_tx_rce),
    .wdata(mim_tx_wdata),
    .raddr(mim_tx_raddr),
    .rdata(mim_tx_rdata)
  );

  pcie_brams_s6 #(
    .NUM_BRAMS        (COLS_RX),
    .RAM_RADDR_LATENCY(TL_RX_RAM_RADDR_LATENCY),
    .RAM_RDATA_LATENCY(TL_RX_RAM_RDATA_LATENCY),
    .RAM_WRITE_LATENCY(TL_RX_RAM_WRITE_LATENCY))
  pcie_brams_rx
  (
    .user_clk_i(user_clk_i),
    .reset_i(reset_i),

    .waddr(mim_rx_waddr),
    .wen(mim_rx_wen),
    .ren(mim_rx_ren),
    .rce(mim_rx_rce),
    .wdata(mim_rx_wdata),
    .raddr(mim_rx_raddr),
    .rdata(mim_rx_rdata)
  );

endmodule // pcie_bram_top
