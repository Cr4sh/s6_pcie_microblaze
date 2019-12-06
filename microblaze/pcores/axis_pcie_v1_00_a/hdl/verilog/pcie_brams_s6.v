//-----------------------------------------------------------------------------
//
// (c) Copyright 2008, 2009 Xilinx, Inc. All rights reserved.
//
// This file contains confidential and proprietary information
// of Xilinx, Inc. and is protected under U.S. and
// international copyright and other intellectual property
// laws.
//
// DISCLAIMER
// This disclaimer is not a license and does not grant any
// rights to the materials distributed herewith. Except as
// otherwise provided in a valid license issued to you by
// Xilinx, and to the maximum extent permitted by applicable
// law: (1) THESE MATERIALS ARE MADE AVAILABLE "AS IS" AND
// WITH ALL FAULTS, AND XILINX HEREBY DISCLAIMS ALL WARRANTIES
// AND CONDITIONS, EXPRESS, IMPLIED, OR STATUTORY, INCLUDING
// BUT NOT LIMITED TO WARRANTIES OF MERCHANTABILITY, NON-
// INFRINGEMENT, OR FITNESS FOR ANY PARTICULAR PURPOSE; and
// (2) Xilinx shall not be liable (whether in contract or tort,
// including negligence, or under any other theory of
// liability) for any loss or damage of any kind or nature
// related to, arising under or in connection with these
// materials, including for any direct, or any indirect,
// special, incidental, or consequential loss or damage
// (including loss of data, profits, goodwill, or any type of
// loss or damage suffered as a result of any action brought
// by a third party) even if such damage or loss was
// reasonably foreseeable or Xilinx had been advised of the
// possibility of the same.
//
// CRITICAL APPLICATIONS
// Xilinx products are not designed or intended to be fail-
// safe, or for use in any application requiring fail-safe
// performance, such as life-support or safety devices or
// systems, Class III medical devices, nuclear facilities,
// applications related to the deployment of airbags, or any
// other applications that could lead to death, personal
// injury, or severe property or environmental damage
// (individually and collectively, "Critical
// Applications"). Customer assumes the sole risk and
// liability of any use of Xilinx products in Critical
// Applications, subject only to applicable laws and
// regulations governing limitations on product liability.
//
// THIS COPYRIGHT NOTICE AND DISCLAIMER MUST BE RETAINED AS
// PART OF THIS FILE AT ALL TIMES.
//
//-----------------------------------------------------------------------------
// Project    : Spartan-6 Integrated Block for PCI Express
// File       : pcie_brams_s6.v
// Description: BlockRAM module for Spartan-6 PCIe Block
//
//              Arranges and connects brams
//              Implements address decoding, datapath muxing and
//              pipeline stages
//
//-----------------------------------------------------------------------------

`timescale 1ns/1ns

module pcie_brams_s6 #(
  // the number of BRAMs to use
  // supported values are:
  // 1,2,4,9
  parameter NUM_BRAMS           = 0,

  // BRAM read address latency
  //
  // value     meaning
  // ====================================================
  //   0       BRAM read address port sample
  //   1       BRAM read address port sample and a pipeline stage on the address port
  parameter RAM_RADDR_LATENCY   = 1,

  // BRAM read data latency
  //
  // value     meaning
  // ====================================================
  //   1       no BRAM OREG
  //   2       use BRAM OREG
  //   3       use BRAM OREG and a pipeline stage on the data port
  parameter RAM_RDATA_LATENCY   = 1,

  // BRAM write latency
  // The BRAM write port is synchronous
  //
  // value     meaning
  // ====================================================
  //   0       BRAM write port sample
  //   1       BRAM write port sample plus pipeline stage
  parameter RAM_WRITE_LATENCY   = 1
) (
  input          user_clk_i,
  input          reset_i,

  input          wen,
  input  [11:0]  waddr,
  input  [35:0]  wdata,
  input          ren,
  input          rce,
  input  [11:0]  raddr,
  output [35:0]  rdata
);

  // turn on the bram output register
  localparam DOB_REG = (RAM_RDATA_LATENCY > 1) ? 1 : 0;

  // calculate the data width of the individual brams
  localparam [6:0] WIDTH = ((NUM_BRAMS == 1) ? 36 :
                            (NUM_BRAMS == 2) ? 18 :
                            (NUM_BRAMS == 4) ?  9 :
                                                4
                           );

  localparam     TCQ     = 1;

  //synthesis translate_off
  initial begin
    case (NUM_BRAMS)
      1,2,4,9:;
      default: begin
        $display("[%t] %m Error NUM_BRAMS %0d not supported", $time, NUM_BRAMS);
        $finish;
      end
    endcase // case(NUM_BRAMS)

    case (RAM_RADDR_LATENCY)
      0,1:;
      default: begin
        $display("[%t] %m Error RAM_READ_LATENCY %0d not supported", $time, RAM_RADDR_LATENCY);
        $finish;
      end
    endcase // case (RAM_RADDR_LATENCY)

    case (RAM_RDATA_LATENCY)
      1,2,3:;
      default: begin
        $display("[%t] %m Error RAM_READ_LATENCY %0d not supported", $time, RAM_RDATA_LATENCY);
        $finish;
      end
    endcase // case (RAM_RDATA_LATENCY)

    case (RAM_WRITE_LATENCY)
      0,1:;
      default: begin
        $display("[%t] %m Error RAM_WRITE_LATENCY %0d not supported", $time, RAM_WRITE_LATENCY);
        $finish;
      end
    endcase // case(RAM_WRITE_LATENCY)
  end
  //synthesis translate_on

  // model the delays for ram write latency

  wire        wen_int;
  wire [11:0] waddr_int;
  wire [35:0] wdata_int;

  generate
    if (RAM_WRITE_LATENCY == 1) begin : wr_lat_2
      reg        wen_dly;
      reg [11:0] waddr_dly;
      reg [35:0] wdata_dly;

      always @(posedge user_clk_i) begin
        if (reset_i) begin
          wen_dly   <= #TCQ 1'b0;
          waddr_dly <= #TCQ 12'b0;
          wdata_dly <= #TCQ 36'b0;
        end else begin
          wen_dly   <= #TCQ wen;
          waddr_dly <= #TCQ waddr;
          wdata_dly <= #TCQ wdata;
        end
      end

      assign wen_int   = wen_dly;
      assign waddr_int = waddr_dly;
      assign wdata_int = wdata_dly;
    end // if (RAM_WRITE_LATENCY == 1)
    else if (RAM_WRITE_LATENCY == 0) begin : wr_lat_1
      assign wen_int   = wen;
      assign waddr_int = waddr;
      assign wdata_int = wdata;
    end
  endgenerate

  // model the delays for ram read latency
  wire        ren_int;
  wire [11:0] raddr_int;
  wire [35:0] rdata_int;

  generate
    if (RAM_RADDR_LATENCY == 1) begin : raddr_lat_2
      reg        ren_dly;
      reg [11:0] raddr_dly;

      always @(posedge user_clk_i) begin
        if (reset_i) begin
          ren_dly   <= #TCQ 1'b0;
          raddr_dly <= #TCQ 12'b0;
        end else begin
          ren_dly   <= #TCQ ren;
          raddr_dly <= #TCQ raddr;
        end // else: !if(reset_i)
      end

      assign ren_int   = ren_dly;
      assign raddr_int = raddr_dly;
    end // block: rd_lat_addr_2
    else begin : raddr_lat_1
      assign ren_int   = ren;
      assign raddr_int = raddr;
    end
  endgenerate

  generate
    if (RAM_RDATA_LATENCY == 3) begin : rdata_lat_3
      reg [35:0] rdata_dly;

      always @(posedge user_clk_i) begin
        if (reset_i) begin
          rdata_dly <= #TCQ 36'b0;
        end else begin
          rdata_dly <= #TCQ rdata_int;
        end // else: !if(reset_i)
      end

      assign rdata     = rdata_dly;

    end // block: rd_lat_data_3
    else begin : rdata_lat_1_2
      assign rdata     = rdata_int;
    end
  endgenerate

  // instantiate the brams
  generate
    genvar i;
    for (i = 0; i < NUM_BRAMS; i = i + 1) begin : brams
      pcie_bram_s6 #(.DOB_REG(DOB_REG), .WIDTH(WIDTH))
        ram (.user_clk_i(user_clk_i), .reset_i(reset_i),
             .wen_i(wen_int), .waddr_i(waddr_int), .wdata_i(wdata_int[(((i + 1) * WIDTH) - 1): (i * WIDTH)]),
             .ren_i(ren_int), .raddr_i(raddr_int), .rdata_o(rdata_int[(((i + 1) * WIDTH) - 1): (i * WIDTH)]), .rce_i(rce));
    end
  endgenerate
endmodule // pcie_brams_s6
