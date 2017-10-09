`timescale 1ps/1ps

module axi_basic_rx #(
  parameter C_DATA_WIDTH  = 128,          // RX/TX interface data width
  parameter C_FAMILY      = "X7",         // Targeted FPGA family
  parameter C_ROOT_PORT   = "FALSE",      // PCIe block is in root port mode
  parameter C_PM_PRIORITY = "FALSE",      // Disable TX packet boundary thrtl
  parameter TCQ = 1,                      // Clock to Q time

  // Do not override parameters below this line
  parameter REM_WIDTH  = (C_DATA_WIDTH == 128) ? 2 : 1, // trem/rrem width
  parameter STRB_WIDTH = C_DATA_WIDTH / 8               // tkeep width
  ) (
  //---------------------------------------------//
  // User Design I/O                             //
  //---------------------------------------------//

  // AXI RX
  //-----------
  output  [C_DATA_WIDTH-1:0] m_axis_rx_tdata,        // RX data to user
  output                     m_axis_rx_tvalid,       // RX data is valid
  input                      m_axis_rx_tready,       // RX ready for data
  output    [STRB_WIDTH-1:0] m_axis_rx_tkeep,        // RX strobe byte enables
  output                     m_axis_rx_tlast,        // RX data is last
  output              [21:0] m_axis_rx_tuser,        // RX user signals

  //---------------------------------------------//
  // PCIe Block I/O                              //
  //---------------------------------------------//

  // TRN RX
  //-----------
  input  [C_DATA_WIDTH-1:0] trn_rd,                  // RX data from block
  input                     trn_rsof,                // RX start of packet
  input                     trn_reof,                // RX end of packet
  input                     trn_rsrc_rdy,            // RX source ready
  output                    trn_rdst_rdy,            // RX destination ready
  input                     trn_rsrc_dsc,            // RX source discontinue
  input     [REM_WIDTH-1:0] trn_rrem,                // RX remainder
  input                     trn_rerrfwd,             // RX error forward
  input               [6:0] trn_rbar_hit,            // RX BAR hit
  input                     trn_recrc_err,           // RX ECRC error

  // System
  //-----------
  output              [2:0] np_counter,              // Non-posted counter
  input                     user_clk,                // user clock from block
  input                     user_rst                 // user reset from block
);


// Wires
wire                  null_rx_tvalid;
wire                  null_rx_tlast;
wire [STRB_WIDTH-1:0] null_rx_tkeep;
wire                  null_rdst_rdy;
wire            [4:0] null_is_eof;

//---------------------------------------------//
// RX Data Pipeline                            //
//---------------------------------------------//

axi_basic_rx_pipeline #(
  .C_DATA_WIDTH( C_DATA_WIDTH ),
  .C_FAMILY( C_FAMILY ),
  .TCQ( TCQ ),

  .REM_WIDTH( REM_WIDTH ),
  .STRB_WIDTH( STRB_WIDTH )

) rx_pipeline_inst (

  // Outgoing AXI TX
  //-----------
  .m_axis_rx_tdata( m_axis_rx_tdata ),
  .m_axis_rx_tvalid( m_axis_rx_tvalid ),
  .m_axis_rx_tready( m_axis_rx_tready ),
  .m_axis_rx_tkeep( m_axis_rx_tkeep ),
  .m_axis_rx_tlast( m_axis_rx_tlast ),
  .m_axis_rx_tuser( m_axis_rx_tuser ),

  // Incoming TRN RX
  //-----------
  .trn_rd( trn_rd ),
  .trn_rsof( trn_rsof ),
  .trn_reof( trn_reof ),
  .trn_rsrc_rdy( trn_rsrc_rdy ),
  .trn_rdst_rdy( trn_rdst_rdy ),
  .trn_rsrc_dsc( trn_rsrc_dsc ),
  .trn_rrem( trn_rrem ),
  .trn_rerrfwd( trn_rerrfwd ),
  .trn_rbar_hit( trn_rbar_hit ),
  .trn_recrc_err( trn_recrc_err ),

  // Null Inputs
  //-----------
  .null_rx_tvalid( null_rx_tvalid ),
  .null_rx_tlast( null_rx_tlast ),
  .null_rx_tkeep( null_rx_tkeep ),
  .null_rdst_rdy( null_rdst_rdy ),
  .null_is_eof( null_is_eof ),

  // System
  //-----------
  .np_counter( np_counter ),
  .user_clk( user_clk ),
  .user_rst( user_rst )
);


 //---------------------------------------------//
 // RX Null Packet Generator                    //
 //---------------------------------------------//

 axi_basic_rx_null_gen #(
  .C_DATA_WIDTH( C_DATA_WIDTH ),
  .TCQ( TCQ ),

  .STRB_WIDTH( STRB_WIDTH )

 ) rx_null_gen_inst (

  // Inputs
  //-----------
  .m_axis_rx_tdata( m_axis_rx_tdata ),
  .m_axis_rx_tvalid( m_axis_rx_tvalid ),
  .m_axis_rx_tready( m_axis_rx_tready ),
  .m_axis_rx_tlast( m_axis_rx_tlast ),
  .m_axis_rx_tuser( m_axis_rx_tuser ),

  // Null Outputs
  //-----------
  .null_rx_tvalid( null_rx_tvalid ),
  .null_rx_tlast( null_rx_tlast ),
  .null_rx_tkeep( null_rx_tkeep ),
  .null_rdst_rdy( null_rdst_rdy ),
  .null_is_eof( null_is_eof ),

  // System
  //-----------
  .user_clk( user_clk ),
  .user_rst( user_rst )
 );

endmodule

