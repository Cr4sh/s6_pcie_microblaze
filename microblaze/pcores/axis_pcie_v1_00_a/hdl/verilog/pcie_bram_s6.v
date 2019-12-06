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
// File       : pcie_bram_s6.v
// Description: BlockRAM module for Spartan-6 PCIe Block
//              The BRAM A port is the write port.
//              The BRAM B port is the read port.
//
//-----------------------------------------------------------------------------

`timescale 1ns/1ns

module pcie_bram_s6 #(
  parameter DOB_REG = 0, // 1 use the output register 0 don't use the output register
  parameter WIDTH = 0    // supported WIDTH's are: 4, 9, 18, 36
) (
  input               user_clk_i,// user clock
  input               reset_i,   // bram reset

  input               wen_i,     // write enable
  input [11:0]        waddr_i,   // write address
  input [WIDTH - 1:0] wdata_i,   // write data

  input               ren_i,     // read enable
  input               rce_i,     // output register clock enable
  input [11:0]        raddr_i,   // read address

  output [WIDTH - 1:0] rdata_o   // read data
);

  // map the address bits
  localparam ADDR_MSB = ((WIDTH == 4)  ? 11 :
                         (WIDTH == 9)  ? 10 :
                         (WIDTH == 18) ?  9 :
                                          8
                         );

  // set the width of the tied off low address bits
  localparam ADDR_LO_BITS = ((WIDTH == 4)  ? 2 :
                             (WIDTH == 9)  ? 3 :
                             (WIDTH == 18) ? 4 :
                                             5
                             );

  localparam WRITE_MODE_INT = "NO_CHANGE";

  //synthesis translate_off
  initial begin
     case (WIDTH)
       4,9,18,36:;
       default:
         begin
            $display("[%t] %m Error WIDTH %0d not supported", $time, WIDTH);
            $finish;
         end
     endcase // case (WIDTH)
  end
  //synthesis translate_on

  wire [31:0] di_wire,  do_wire;
  wire [3:0]  dip_wire, dop_wire;
  generate
    case (WIDTH)
      36: begin : width_36
        assign di_wire  = wdata_i[31:0];
        assign dip_wire = wdata_i[35:32];
        assign rdata_o  = {dop_wire, do_wire};
      end
      18: begin : width_18
        assign di_wire  = {16'd0, wdata_i[15:0]};
        assign dip_wire = {2'd0,  wdata_i[17:16]};
        assign rdata_o  = {dop_wire[1:0], do_wire[15:0]};
      end
      9:  begin : width_9
        assign di_wire  = {24'd0, wdata_i[7:0]};
        assign dip_wire = {3'd0,  wdata_i[8]};
        assign rdata_o  = {dop_wire[0], do_wire[7:0]};
      end
      4:  begin : width_4
        assign di_wire  = {28'd0, wdata_i[3:0]};
        assign dip_wire = 4'd0;
        assign rdata_o  = do_wire[3:0];
      end
    endcase
  endgenerate

  RAMB16BWER #(
    .SIM_DEVICE     ("SPARTAN6"),
    .DATA_WIDTH_A  (WIDTH),
    .DATA_WIDTH_B  (WIDTH),
    .DOA_REG       (0),
    .DOB_REG       (DOB_REG),
    .WRITE_MODE_A  (WRITE_MODE_INT),
    .WRITE_MODE_B  (WRITE_MODE_INT)
  ) ramb16 (
    .CLKA          (user_clk_i),
    .RSTA          (reset_i),
    .DOA           (),
    .DOPA          (),
    .ADDRA         ({waddr_i[ADDR_MSB:0], {ADDR_LO_BITS{1'b0}}}),
    .DIA           (di_wire),
    .DIPA          (dip_wire),
    .ENA           (wen_i),
    .WEA           ({4{wen_i}}),
    .REGCEA        (1'b0),

    .CLKB          (user_clk_i),
    .RSTB          (reset_i),
    .WEB           (4'd0),
    .DIB           (32'd0),
    .DIPB          (4'd0),
    .ADDRB         ({raddr_i[ADDR_MSB:0], {ADDR_LO_BITS{1'b0}}}),
    .DOB           (do_wire),
    .DOPB          (dop_wire),
    .ENB           (ren_i),
    .REGCEB        (rce_i)
  );

endmodule // pcie_bram_s6
