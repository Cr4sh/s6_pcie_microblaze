`timescale 1ns / 1ps

module axis_pcie #(

  parameter FAST_TRAIN = "FALSE"

) (

  input                  PCIE_CLK_N,
  input                  PCIE_CLK_P,

  output                 PCIE_TXP,
  output                 PCIE_TXN,
  input                  PCIE_RXP,
  input                  PCIE_RXN,
  
  output     [3 : 0]     LED,
  
  input                  ACLK,
  input                  ARESETN,
  
  output                 S0_AXIS_TREADY,
  input      [31 : 0]    S0_AXIS_TDATA,
  input                  S0_AXIS_TLAST,
  input                  S0_AXIS_TVALID,

  output                 M0_AXIS_TVALID,
  output     [31 : 0]    M0_AXIS_TDATA,
  output                 M0_AXIS_TLAST,
  input                  M0_AXIS_TREADY,

  output                 S1_AXIS_TREADY,
  input      [31 : 0]    S1_AXIS_TDATA,
  input                  S1_AXIS_TLAST,
  input                  S1_AXIS_TVALID,

  output                 M1_AXIS_TVALID,
  output     [31 : 0]    M1_AXIS_TDATA,
  output                 M1_AXIS_TLAST,
  input                  M1_AXIS_TREADY,

  output                 S2_AXIS_TREADY,
  input      [31 : 0]    S2_AXIS_TDATA,
  input                  S2_AXIS_TLAST,
  input                  S2_AXIS_TVALID,

  output                 M2_AXIS_TVALID,
  output     [31 : 0]    M2_AXIS_TDATA,
  output                 M2_AXIS_TLAST,
  input                  M2_AXIS_TREADY
);

  //
  // PCI-E interface
  //

  // common
  wire          user_clk;
  wire          user_reset;
  wire          user_lnk_up;

  // tx
  wire [5:0]    tx_buf_av;
  wire          tx_cfg_req;
  wire          tx_err_drop;
  wire          tx_cfg_gnt;
  wire          s_axis_tx_tready;
  wire [3:0]    s_axis_tx_tuser;
  wire [31:0]   s_axis_tx_tdata;
  wire [3:0]    s_axis_tx_tkeep;
  wire          s_axis_tx_tlast;
  wire          s_axis_tx_tvalid;

  // rx
  wire [31:0]   m_axis_rx_tdata;
  wire [3:0]    m_axis_rx_tkeep;
  wire          m_axis_rx_tlast;
  wire          m_axis_rx_tvalid;
  wire          m_axis_rx_tready;
  wire  [21:0]  m_axis_rx_tuser;
  wire          rx_np_ok;

  // flow control
  wire [11:0]   fc_cpld;
  wire [7:0]    fc_cplh;
  wire [11:0]   fc_npd;
  wire [7:0]    fc_nph;
  wire [11:0]   fc_pd;
  wire [7:0]    fc_ph;
  wire [2:0]    fc_sel;

  // configuration
  wire [63:0]   cfg_dsn;
  wire [31:0]   cfg_do;
  wire          cfg_rd_wr_done;
  wire [9:0]    cfg_dwaddr;
  wire          cfg_rd_en;

  // error signaling
  wire          cfg_err_cor;
  wire          cfg_err_ur;
  wire          cfg_err_ecrc;
  wire          cfg_err_cpl_timeout;
  wire          cfg_err_cpl_abort;
  wire          cfg_err_posted;
  wire          cfg_err_locked;
  wire [47:0]   cfg_err_tlp_cpl_header;
  wire          cfg_err_cpl_rdy;

  // interrupt signaling
  wire          cfg_interrupt;
  wire          cfg_interrupt_rdy;
  wire          cfg_interrupt_assert;
  wire [7:0]    cfg_interrupt_di;
  wire [7:0]    cfg_interrupt_do;
  wire [2:0]    cfg_interrupt_mmenable;
  wire          cfg_interrupt_msienable;

  // power management signaling
  wire          cfg_turnoff_ok;
  wire          cfg_to_turnoff;
  wire          cfg_trn_pending;
  wire          cfg_pm_wake;

  // system configuration and status
  wire [7:0]    cfg_bus_number;
  wire [4:0]    cfg_device_number;
  wire [2:0]    cfg_function_number;
  wire [15:0]   cfg_status;
  wire [15:0]   cfg_command;
  wire [15:0]   cfg_dstatus;
  wire [15:0]   cfg_dcommand;
  wire [15:0]   cfg_lstatus;
  wire [15:0]   cfg_lcommand;
  wire [2:0]    cfg_pcie_link_state;

  // system interface
  wire          sys_clk_c;
  wire          sys_reset;  

  // system reset
  assign sys_reset = !ARESETN;

  wire          bus_ready;

  assign bus_ready = (cfg_bus_number == 0 && cfg_device_number == 0 && cfg_function_number == 0) ? 0 : 1;

  //
  // input and output buffers
  //

  // buffer for differential system clock
  IBUFDS refclk_ibuf(.O( sys_clk_c ), .I( PCIE_CLK_P ), .IB( PCIE_CLK_N ));  

  // buffers for diagnostic LEDs
  OBUF led_0_obuf(.O( LED[0] ), .I( user_lnk_up ));
  OBUF led_1_obuf(.O( LED[1] ), .I( bus_ready ));
  OBUF led_2_obuf(.O( LED[2] ), .I( user_reset ));   

  application app(

    //
    // Microblaze interfaces
    //

    .clk( ACLK ),

    // tlp tx
    .s_axis_mb0_tx_tready( M0_AXIS_TREADY ),
    .s_axis_mb0_tx_tdata( M0_AXIS_TDATA ),
    .s_axis_mb0_tx_tlast( M0_AXIS_TLAST ),
    .s_axis_mb0_tx_tvalid( M0_AXIS_TVALID ),

    // tlp rx  
    .m_axis_mb0_rx_tdata( S0_AXIS_TDATA ),
    .m_axis_mb0_rx_tlast( S0_AXIS_TLAST ),
    .m_axis_mb0_rx_tvalid( S0_AXIS_TVALID ),
    .m_axis_mb0_rx_tready( S0_AXIS_TREADY ),

    // info tx
    .s_axis_mb1_tx_tready( M1_AXIS_TREADY ),
    .s_axis_mb1_tx_tdata( M1_AXIS_TDATA ),
    .s_axis_mb1_tx_tlast( M1_AXIS_TLAST ),
    .s_axis_mb1_tx_tvalid( M1_AXIS_TVALID ),

    // info rx  
    .m_axis_mb1_rx_tdata( S1_AXIS_TDATA ),
    .m_axis_mb1_rx_tlast( S1_AXIS_TLAST ),
    .m_axis_mb1_rx_tvalid( S1_AXIS_TVALID ),
    .m_axis_mb1_rx_tready( S1_AXIS_TREADY ),

    // cfg tx
    .s_axis_mb2_tx_tready( M2_AXIS_TREADY ),
    .s_axis_mb2_tx_tdata( M2_AXIS_TDATA ),
    .s_axis_mb2_tx_tlast( M2_AXIS_TLAST ),
    .s_axis_mb2_tx_tvalid( M2_AXIS_TVALID ),

    // cfg rx  
    .m_axis_mb2_rx_tdata( S2_AXIS_TDATA ),
    .m_axis_mb2_rx_tlast( S2_AXIS_TLAST ),
    .m_axis_mb2_rx_tvalid( S2_AXIS_TVALID ),
    .m_axis_mb2_rx_tready( S2_AXIS_TREADY ),

    .led(LED[3]),
  
    //
    // PCI-E transaction interface
    //

    // common lock & reset
    .user_clk( user_clk ),
    .user_reset( user_reset ),
    .user_lnk_up( user_lnk_up ),    
     
    // common flow control
    .fc_cpld( fc_cpld ),
    .fc_cplh( fc_cplh ),
    .fc_npd( fc_npd ),
    .fc_nph( fc_nph ),
    .fc_pd( fc_pd ),
    .fc_ph( fc_ph ),
    .fc_sel( fc_sel ),
     
    // transaction Tx
    .tx_buf_av( tx_buf_av ),
    .tx_cfg_req( tx_cfg_req ),
    .tx_cfg_gnt( tx_cfg_gnt ),
    .tx_err_drop( tx_err_drop ),
    .s_axis_pcie_tx_tready( s_axis_tx_tready ),
    .s_axis_pcie_tx_tdata( s_axis_tx_tdata ),
    .s_axis_pcie_tx_tkeep( s_axis_tx_tkeep ),
    .s_axis_pcie_tx_tuser( s_axis_tx_tuser ),
    .s_axis_pcie_tx_tlast( s_axis_tx_tlast ),
    .s_axis_pcie_tx_tvalid( s_axis_tx_tvalid ),

    // transaction Rx
    .m_axis_pcie_rx_tdata( m_axis_rx_tdata ),
    .m_axis_pcie_rx_tkeep( m_axis_rx_tkeep ),
    .m_axis_pcie_rx_tlast( m_axis_rx_tlast ),
    .m_axis_pcie_rx_tvalid( m_axis_rx_tvalid ),
    .m_axis_pcie_rx_tready( m_axis_rx_tready ),
    .m_axis_pcie_rx_tuser ( m_axis_rx_tuser ),
    .rx_np_ok( rx_np_ok ),

    //
    // PCI-E configuration interface
    //

    // configuration space access
    .cfg_do( cfg_do ),
    .cfg_rd_wr_done( cfg_rd_wr_done ),
    .cfg_dwaddr( cfg_dwaddr ),
    .cfg_rd_en( cfg_rd_en ),
     
    // error signaling
    .cfg_err_cor( cfg_err_cor ),
    .cfg_err_ur( cfg_err_ur ),
    .cfg_err_ecrc( cfg_err_ecrc ),
    .cfg_err_cpl_timeout( cfg_err_cpl_timeout ),
    .cfg_err_cpl_abort( cfg_err_cpl_abort ),
    .cfg_err_posted( cfg_err_posted ),
    .cfg_err_locked( cfg_err_locked ),
    .cfg_err_tlp_cpl_header( cfg_err_tlp_cpl_header ),
    .cfg_err_cpl_rdy( cfg_err_cpl_rdy ),
     
    // interrupt generation
    .cfg_interrupt( cfg_interrupt ),
    .cfg_interrupt_rdy( cfg_interrupt_rdy ),
    .cfg_interrupt_assert( cfg_interrupt_assert ),
    .cfg_interrupt_di( cfg_interrupt_di ),
    .cfg_interrupt_do( cfg_interrupt_do ),
    .cfg_interrupt_mmenable( cfg_interrupt_mmenable ),
    .cfg_interrupt_msienable( cfg_interrupt_msienable ),
    
     // power managemnt signaling
    .cfg_turnoff_ok( cfg_turnoff_ok ),
    .cfg_to_turnoff( cfg_to_turnoff ),
    .cfg_trn_pending( cfg_trn_pending ),
    .cfg_pm_wake( cfg_pm_wake ),
    
     // system configuration and status
    .cfg_bus_number( cfg_bus_number ),
    .cfg_device_number( cfg_device_number ),
    .cfg_function_number( cfg_function_number ),
    .cfg_status( cfg_status ),
    .cfg_command( cfg_command ),
    .cfg_dstatus( cfg_dstatus ),
    .cfg_dcommand( cfg_dcommand ),
    .cfg_lstatus( cfg_lstatus ),
    .cfg_lcommand( cfg_lcommand ),
    .cfg_pcie_link_state( cfg_pcie_link_state ),
    .cfg_dsn( cfg_dsn )
  );

  s6_pcie_v2_4 #(
  
    .FAST_TRAIN                         ( FAST_TRAIN                              )
  
  ) s6_pcie_v2_4_i (

    //
    // PCI-E fabric interface
    //
    .pci_exp_txp                        ( PCIE_TXP                                ),
    .pci_exp_txn                        ( PCIE_TXN                                ),
    .pci_exp_rxp                        ( PCIE_RXP                                ),
    .pci_exp_rxn                        ( PCIE_RXN                                ),

    //
    // transaction interface
    //

    // common clock & reset
    .user_lnk_up                        ( user_lnk_up                             ),
    .user_clk_out                       ( user_clk                                ),
    .user_reset_out                     ( user_reset                              ),
    
    // common flow control
    .fc_sel                             ( fc_sel                                  ),
    .fc_nph                             ( fc_nph                                  ),
    .fc_npd                             ( fc_npd                                  ),
    .fc_ph                              ( fc_ph                                   ),
    .fc_pd                              ( fc_pd                                   ),
    .fc_cplh                            ( fc_cplh                                 ),
    .fc_cpld                            ( fc_cpld                                 ),

    // transaction Tx
    .s_axis_tx_tready                   ( s_axis_tx_tready                        ),
    .s_axis_tx_tdata                    ( s_axis_tx_tdata                         ),
    .s_axis_tx_tkeep                    ( s_axis_tx_tkeep                         ),
    .s_axis_tx_tuser                    ( s_axis_tx_tuser                         ),
    .s_axis_tx_tlast                    ( s_axis_tx_tlast                         ),
    .s_axis_tx_tvalid                   ( s_axis_tx_tvalid                        ),
    .tx_err_drop                        ( tx_err_drop                             ),
    .tx_buf_av                          ( tx_buf_av                               ),
    .tx_cfg_req                         ( tx_cfg_req                              ),
    .tx_cfg_gnt                         ( tx_cfg_gnt                              ),

    // transaction Rx
    .m_axis_rx_tdata                    ( m_axis_rx_tdata                         ),
    .m_axis_rx_tkeep                    ( m_axis_rx_tkeep                         ),
    .m_axis_rx_tlast                    ( m_axis_rx_tlast                         ),
    .m_axis_rx_tvalid                   ( m_axis_rx_tvalid                        ),
    .m_axis_rx_tready                   ( m_axis_rx_tready                        ),
    .m_axis_rx_tuser                    ( m_axis_rx_tuser                         ),
    .rx_np_ok                           ( rx_np_ok                                ),

    //
    // configuration interface
    //

    // configuration space access
    .cfg_do                             ( cfg_do                                  ),
    .cfg_rd_wr_done                     ( cfg_rd_wr_done                          ),
    .cfg_dwaddr                         ( cfg_dwaddr                              ),
    .cfg_rd_en                          ( cfg_rd_en                               ),
    
     // error reporting
    .cfg_err_ur                         ( cfg_err_ur                              ),
    .cfg_err_cor                        ( cfg_err_cor                             ),
    .cfg_err_ecrc                       ( cfg_err_ecrc                            ),
    .cfg_err_cpl_timeout                ( cfg_err_cpl_timeout                     ),
    .cfg_err_cpl_abort                  ( cfg_err_cpl_abort                       ),
    .cfg_err_posted                     ( cfg_err_posted                          ),
    .cfg_err_locked                     ( cfg_err_locked                          ),
    .cfg_err_tlp_cpl_header             ( cfg_err_tlp_cpl_header                  ),
    .cfg_err_cpl_rdy                    ( cfg_err_cpl_rdy                         ),
    
     // interrupt generation
    .cfg_interrupt                      ( cfg_interrupt                           ),
    .cfg_interrupt_rdy                  ( cfg_interrupt_rdy                       ),
    .cfg_interrupt_assert               ( cfg_interrupt_assert                    ),
    .cfg_interrupt_do                   ( cfg_interrupt_do                        ),
    .cfg_interrupt_di                   ( cfg_interrupt_di                        ),
    .cfg_interrupt_mmenable             ( cfg_interrupt_mmenable                  ),
    .cfg_interrupt_msienable            ( cfg_interrupt_msienable                 ),
    
     // power management signaling
    .cfg_turnoff_ok                     ( cfg_turnoff_ok                          ),
    .cfg_to_turnoff                     ( cfg_to_turnoff                          ),
    .cfg_pm_wake                        ( cfg_pm_wake                             ),
    .cfg_pcie_link_state                ( cfg_pcie_link_state                     ),
    .cfg_trn_pending                    ( cfg_trn_pending                         ),
    
     // system configuration and status
    .cfg_dsn                            ( cfg_dsn                                 ),
    .cfg_bus_number                     ( cfg_bus_number                          ),
    .cfg_device_number                  ( cfg_device_number                       ),
    .cfg_function_number                ( cfg_function_number                     ),
    .cfg_status                         ( cfg_status                              ),
    .cfg_command                        ( cfg_command                             ),
    .cfg_dstatus                        ( cfg_dstatus                             ),
    .cfg_dcommand                       ( cfg_dcommand                            ),
    .cfg_lstatus                        ( cfg_lstatus                             ),
    .cfg_lcommand                       ( cfg_lcommand                            ),

    //
    // system interface
    //
    .sys_clk                            ( sys_clk_c                               ),
    .sys_reset                          ( sys_reset                               ),
    .received_hot_reset                 (                                         )
  );

endmodule // axis_pcie
