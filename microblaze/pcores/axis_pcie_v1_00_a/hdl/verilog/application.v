`timescale 1ns / 1ps

module application
(
  /*
      Microblaze peripheral signals
   */

  // microblaze clock
  input            clk,

  // tlp tx
  input            s_axis_mb0_tx_tready,
  output  [31:0]   s_axis_mb0_tx_tdata,
  output           s_axis_mb0_tx_tlast,
  output           s_axis_mb0_tx_tvalid,

  // tlp rx  
  input  [31:0]    m_axis_mb0_rx_tdata,
  input            m_axis_mb0_rx_tlast,
  input            m_axis_mb0_rx_tvalid,
  output           m_axis_mb0_rx_tready,

  // info tx
  input            s_axis_mb1_tx_tready,
  output  [31:0]   s_axis_mb1_tx_tdata,
  output           s_axis_mb1_tx_tlast,
  output           s_axis_mb1_tx_tvalid,

  // info rx
  input  [31:0]    m_axis_mb1_rx_tdata,
  input            m_axis_mb1_rx_tlast,
  input            m_axis_mb1_rx_tvalid,
  output           m_axis_mb1_rx_tready,

  // cfg tx
  input            s_axis_mb2_tx_tready,
  output  [31:0]   s_axis_mb2_tx_tdata,
  output           s_axis_mb2_tx_tlast,
  output           s_axis_mb2_tx_tvalid,

  // cfg rx
  input  [31:0]    m_axis_mb2_rx_tdata,
  input            m_axis_mb2_rx_tlast,
  input            m_axis_mb2_rx_tvalid,
  output           m_axis_mb2_rx_tready,

  output           led,

   /*
      PCI-E signals
   */

  // common
  input            user_clk,
  input            user_reset,
  input            user_lnk_up,

  // tx
  input  [5:0]     tx_buf_av,
  input            tx_cfg_req,
  output           tx_cfg_gnt,
  input            tx_err_drop,

  input            s_axis_pcie_tx_tready,
  output  [31:0]   s_axis_pcie_tx_tdata,
  output  [3:0]    s_axis_pcie_tx_tkeep,
  output  [3:0]    s_axis_pcie_tx_tuser,
  output           s_axis_pcie_tx_tlast,
  output           s_axis_pcie_tx_tvalid,

  // rx  
  output           rx_np_ok,
  input  [31:0]    m_axis_pcie_rx_tdata,
  input  [3:0]     m_axis_pcie_rx_tkeep,
  input            m_axis_pcie_rx_tlast,
  input            m_axis_pcie_rx_tvalid,
  output           m_axis_pcie_rx_tready,
  input  [21:0]    m_axis_pcie_rx_tuser,

  // flow control
  input  [11:0]    fc_cpld,
  input  [7:0]     fc_cplh,
  input  [11:0]    fc_npd,
  input  [7:0]     fc_nph,
  input  [11:0]    fc_pd,
  input  [7:0]     fc_ph,
  output [2:0]     fc_sel,

  // configuration
  input  [31:0]    cfg_do,
  input            cfg_rd_wr_done,
  output [9:0]     cfg_dwaddr,
  output           cfg_rd_en,
  output           cfg_err_cor,
  output           cfg_err_ur,
  output           cfg_err_ecrc,
  output           cfg_err_cpl_timeout,
  output           cfg_err_cpl_abort,
  output           cfg_err_posted,
  output           cfg_err_locked,
  output [47:0]    cfg_err_tlp_cpl_header,
  input            cfg_err_cpl_rdy,
  output           cfg_interrupt,
  input            cfg_interrupt_rdy,
  output           cfg_interrupt_assert,
  output [7:0]     cfg_interrupt_di,
  input  [7:0]     cfg_interrupt_do,
  input  [2:0]     cfg_interrupt_mmenable,
  input            cfg_interrupt_msienable,
  output           cfg_turnoff_ok,
  input            cfg_to_turnoff,
  output           cfg_trn_pending,
  output           cfg_pm_wake,
  input   [7:0]    cfg_bus_number,
  input   [4:0]    cfg_device_number,
  input   [2:0]    cfg_function_number,
  input  [15:0]    cfg_status,
  input  [15:0]    cfg_command,
  input  [15:0]    cfg_dstatus,
  input  [15:0]    cfg_dcommand,
  input  [15:0]    cfg_lstatus,
  input  [15:0]    cfg_lcommand,
  input   [2:0]    cfg_pcie_link_state,
  output [63:0]    cfg_dsn
);

  localparam PCI_EXP_EP_OUI = 24'h000A35;
  localparam PCI_EXP_EP_DSN_1 = { { 8'h1 }, PCI_EXP_EP_OUI };
  localparam PCI_EXP_EP_DSN_2 = 32'h00000001;

  localparam PCIE_UART_S_RX = 1'b0;
  localparam PCIE_UART_S_TX = 1'b1;

  //
  // bits of command register
  //

  wire cfg_command_interrupt_disable;
  wire cfg_command_serr_en;
  wire cfg_command_bus_master_enable;
  wire cfg_command_mem_enable;
  wire cfg_command_io_enable;

  assign cfg_command_interrupt_disable = cfg_command[10];
  assign cfg_command_serr_en = cfg_command[8];
  assign cfg_command_bus_master_enable = cfg_command[2];
  assign cfg_command_mem_enable = cfg_command[1];
  assign cfg_command_io_enable = cfg_command[0];  

  //
  // PCI-E core input tie-offs
  //

  assign fc_sel = 3'b0;

  assign rx_np_ok = 1'b1;
  assign tx_cfg_gnt = 1'b1;

  assign cfg_err_cor           = 1'b0;
  assign cfg_err_ur            = 1'b0;
  assign cfg_err_ecrc          = 1'b0;
  assign cfg_err_cpl_timeout   = 1'b0;
  assign cfg_err_cpl_abort     = 1'b0;
  assign cfg_err_posted        = 1'b0;
  assign cfg_err_locked        = 1'b0;
  assign cfg_pm_wake           = 1'b0;
  assign cfg_trn_pending       = 1'b0;  

  assign cfg_interrupt_assert  = 1'b0;
  assign cfg_interrupt         = 1'b0;
  assign cfg_interrupt_di      = 8'b0;
  
  assign cfg_turnoff_ok = 1'b0;

  assign cfg_err_tlp_cpl_header = 48'h0;  
  assign cfg_dsn = { PCI_EXP_EP_DSN_2, PCI_EXP_EP_DSN_1 };    
  
  //
  // FIFO
  //

  wire s_aresetn;

  assign s_aresetn = !user_reset;

  //
  // RX interface
  //  
  
  fifo_generator_v8_4 fifo_rx(
    .m_aclk( clk ),
    .s_aclk( user_clk ),
    .s_aresetn( s_aresetn ),
    .s_axis_tvalid( m_axis_pcie_rx_tvalid ),
    .s_axis_tready( m_axis_pcie_rx_tready ),
    .s_axis_tdata( m_axis_pcie_rx_tdata ),
    .s_axis_tlast( m_axis_pcie_rx_tlast ),
    .m_axis_tvalid( s_axis_mb0_tx_tvalid ),
    .m_axis_tready( s_axis_mb0_tx_tready ),
    .m_axis_tdata( s_axis_mb0_tx_tdata ),
    .m_axis_tlast( s_axis_mb0_tx_tlast )
  );
  
  //
  // TX interface
  // 
  
  // not used on spartan 6, must be 0
  assign s_axis_pcie_tx_tuser[2:0] = 3'b0;

  // not used on spartan 6, must be 0xf
  assign s_axis_pcie_tx_tkeep = 4'hf;  

  fifo_generator_v8_4 fifo_tx(
    .m_aclk( user_clk ),
    .s_aclk( clk ),
    .s_aresetn( s_aresetn ),
    .s_axis_tvalid( m_axis_mb0_rx_tvalid ),
    .s_axis_tready( m_axis_mb0_rx_tready ),
    .s_axis_tdata( m_axis_mb0_rx_tdata ),
    .s_axis_tlast( m_axis_mb0_rx_tlast ),
    .m_axis_tvalid( s_axis_pcie_tx_tvalid ),
    .m_axis_tready( s_axis_pcie_tx_tready ),
    .m_axis_tdata( s_axis_pcie_tx_tdata ),
    .m_axis_tlast( s_axis_pcie_tx_tlast )
  );

  //
  // Link info
  //

  // tx
  assign s_axis_mb1_tx_tlast = 1'b1;
  assign s_axis_mb1_tx_tvalid = 1'b1;
  assign s_axis_mb1_tx_tdata = { 

    16'h0, 

    // actual PCI-E device address
    cfg_bus_number,
    cfg_device_number, 
    cfg_function_number
  };

  reg r_led = 1'b0;

  assign led = r_led;
  assign m_axis_mb1_rx_tready = 1'b1;

  always @ (posedge clk) begin

    if (m_axis_mb1_rx_tvalid == 1'b1) begin

      r_led <= m_axis_mb1_rx_tdata[0];
      
    end
  end

  //
  // Config space RX
  //

  reg s_axis_cfg_rx_tlast = 1'b1;

  fifo_generator_v8_4 fifo_cfg_rx(
    .m_aclk( clk ),
    .s_aclk( user_clk ),
    .s_aresetn( s_aresetn ),
    .s_axis_tvalid( cfg_rd_wr_done ),
    .s_axis_tdata( cfg_do ),
    .s_axis_tlast( s_axis_cfg_rx_tlast ),
    .m_axis_tvalid( s_axis_mb2_tx_tvalid ),
    .m_axis_tready( s_axis_mb2_tx_tready ),
    .m_axis_tdata( s_axis_mb2_tx_tdata ),
    .m_axis_tlast( s_axis_mb2_tx_tlast )
  );

  //
  // Config space TX
  //

  reg m_axis_cfg_tx_tready = 1'b1;  

  wire [31:0] m_axis_cfg_tx_tdata;

  assign cfg_dwaddr = m_axis_cfg_tx_tdata[9:0];

  fifo_generator_v8_4 fifo_cfg_tx(
    .m_aclk( user_clk ),
    .s_aclk( clk ),
    .s_aresetn( s_aresetn ),
    .s_axis_tvalid( m_axis_mb2_rx_tvalid ),
    .s_axis_tready( m_axis_mb2_rx_tready ),
    .s_axis_tdata( m_axis_mb2_rx_tdata ),
    .s_axis_tlast( m_axis_mb2_rx_tlast ),
    .m_axis_tvalid( cfg_rd_en ),    
    .m_axis_tdata( m_axis_cfg_tx_tdata ),
    .m_axis_tready( m_axis_cfg_tx_tready )
  );
  

endmodule // application

