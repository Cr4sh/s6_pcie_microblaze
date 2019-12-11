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
// File       : s6_pcie_v2_4.v
// Description: Spartan-6 solution wrapper : Endpoint for PCI Express
//
//-----------------------------------------------------------------------------

`timescale 1ns/1ns

(* CORE_GENERATION_INFO = "s6_pcie_v2_4,s6_pcie_v2_4,{TL_TX_RAM_RADDR_LATENCY=0,TL_TX_RAM_RDATA_LATENCY=2,TL_RX_RAM_RADDR_LATENCY=0,TL_RX_RAM_RDATA_LATENCY=2,TL_RX_RAM_WRITE_LATENCY=0,VC0_TX_LASTPACKET=29,VC0_RX_RAM_LIMIT=FFF,VC0_TOTAL_CREDITS_PH=32,VC0_TOTAL_CREDITS_PD=467,VC0_TOTAL_CREDITS_NPH=8,VC0_TOTAL_CREDITS_CH=40,VC0_TOTAL_CREDITS_CD=467,VC0_CPL_INFINITE=TRUE,BAR0=FFF00000,BAR1=00000000,BAR2=00000000,BAR3=00000000,BAR4=00000000,BAR5=00000000,EXPANSION_ROM=000000,USR_CFG=FALSE,USR_EXT_CFG=FALSE,DEV_CAP_MAX_PAYLOAD_SUPPORTED=2,CLASS_CODE=020000,CARDBUS_CIS_POINTER=00000000,PCIE_CAP_CAPABILITY_VERSION=1,PCIE_CAP_DEVICE_PORT_TYPE=0,DEV_CAP_PHANTOM_FUNCTIONS_SUPPORT=0,DEV_CAP_EXT_TAG_SUPPORTED=FALSE,DEV_CAP_ENDPOINT_L0S_LATENCY=7,DEV_CAP_ENDPOINT_L1_LATENCY=7,LINK_CAP_ASPM_SUPPORT=1,MSI_CAP_MULTIMSGCAP=0,MSI_CAP_MULTIMSG_EXTENSION=0,LINK_STATUS_SLOT_CLOCK_CONFIG=FALSE,ENABLE_RX_TD_ECRC_TRIM=TRUE,DISABLE_SCRAMBLING=FALSE,PM_CAP_DSI=FALSE,PM_CAP_D1SUPPORT=TRUE,PM_CAP_D2SUPPORT=TRUE,PM_CAP_PMESUPPORT=0F,PM_DATA0=00,PM_DATA_SCALE0=0,PM_DATA1=00,PM_DATA_SCALE1=0,PM_DATA2=00,PM_DATA_SCALE2=0,PM_DATA3=00,PM_DATA_SCALE3=0,PM_DATA4=00,PM_DATA_SCALE4=0,PM_DATA5=00,PM_DATA_SCALE5=0,PM_DATA6=00,PM_DATA_SCALE6=0,PM_DATA7=00,PM_DATA_SCALE7=0,PCIE_GENERIC=000010101111,GTP_SEL=0,CFG_VEN_ID=10EE,CFG_DEV_ID=1337,CFG_REV_ID=00,CFG_SUBSYS_VEN_ID=10EE,CFG_SUBSYS_ID=0007,REF_CLK_FREQ=1}" *)
module s6_pcie_v2_4
 #(
  parameter   [0:0] TL_TX_RAM_RADDR_LATENCY           = 0,
  parameter   [1:0] TL_TX_RAM_RDATA_LATENCY           = 2,
  parameter   [0:0] TL_RX_RAM_RADDR_LATENCY           = 0,
  parameter   [1:0] TL_RX_RAM_RDATA_LATENCY           = 2,
  parameter   [0:0] TL_RX_RAM_WRITE_LATENCY           = 0,
  parameter   [4:0] VC0_TX_LASTPACKET                 = 29,
  parameter  [11:0] VC0_RX_RAM_LIMIT                  = 12'hFFF,
  parameter   [6:0] VC0_TOTAL_CREDITS_PH              = 32,
  parameter  [10:0] VC0_TOTAL_CREDITS_PD              = 467,
  parameter   [6:0] VC0_TOTAL_CREDITS_NPH             = 8,
  parameter   [6:0] VC0_TOTAL_CREDITS_CH              = 40,
  parameter  [10:0] VC0_TOTAL_CREDITS_CD              = 467,
  parameter         VC0_CPL_INFINITE                  = "TRUE",
  parameter  [31:0] BAR0                              = 32'hFFF00000,
  parameter  [31:0] BAR1                              = 32'h00000000,
  parameter  [31:0] BAR2                              = 32'h00000000,
  parameter  [31:0] BAR3                              = 32'h00000000,
  parameter  [31:0] BAR4                              = 32'h00000000,
  parameter  [31:0] BAR5                              = 32'h00000000,
  parameter  [21:0] EXPANSION_ROM                     = 22'h000000,
  parameter         DISABLE_BAR_FILTERING             = "FALSE",
  parameter         DISABLE_ID_CHECK                  = "FALSE",
  parameter         TL_TFC_DISABLE                    = "FALSE",
  parameter         TL_TX_CHECKS_DISABLE              = "FALSE",
  parameter         USR_CFG                           = "FALSE",
  parameter         USR_EXT_CFG                       = "FALSE",
  parameter   [2:0] DEV_CAP_MAX_PAYLOAD_SUPPORTED     = 3'd2,
  parameter  [23:0] CLASS_CODE                        = 24'h020000,
  parameter  [31:0] CARDBUS_CIS_POINTER               = 32'h00000000,
  parameter   [3:0] PCIE_CAP_CAPABILITY_VERSION       = 4'h1,
  parameter   [3:0] PCIE_CAP_DEVICE_PORT_TYPE         = 4'h0,
  parameter         PCIE_CAP_SLOT_IMPLEMENTED         = "FALSE",
  parameter   [4:0] PCIE_CAP_INT_MSG_NUM              = 5'b00000,
  parameter   [1:0] DEV_CAP_PHANTOM_FUNCTIONS_SUPPORT = 2'd0,
  parameter         DEV_CAP_EXT_TAG_SUPPORTED         = "FALSE",
  parameter   [2:0] DEV_CAP_ENDPOINT_L0S_LATENCY      = 3'd7,
  parameter   [2:0] DEV_CAP_ENDPOINT_L1_LATENCY       = 3'd7,
  parameter         SLOT_CAP_ATT_BUTTON_PRESENT       = "FALSE",
  parameter         SLOT_CAP_ATT_INDICATOR_PRESENT    = "FALSE",
  parameter         SLOT_CAP_POWER_INDICATOR_PRESENT  = "FALSE",
  parameter         DEV_CAP_ROLE_BASED_ERROR          = "TRUE",
  parameter   [1:0] LINK_CAP_ASPM_SUPPORT             = 2'd1,
  parameter   [2:0] LINK_CAP_L0S_EXIT_LATENCY         = 3'd7,
  parameter   [2:0] LINK_CAP_L1_EXIT_LATENCY          = 3'd7,
  parameter  [14:0] LL_ACK_TIMEOUT                    = 15'h00B7,
  parameter         LL_ACK_TIMEOUT_EN                 = "FALSE",
  parameter  [14:0] LL_REPLAY_TIMEOUT                 = 15'h00FF,
  parameter         LL_REPLAY_TIMEOUT_EN              = "TRUE",
  parameter   [2:0] MSI_CAP_MULTIMSGCAP               = 3'd0,
  parameter   [0:0] MSI_CAP_MULTIMSG_EXTENSION        = 1'd0,
  parameter         LINK_STATUS_SLOT_CLOCK_CONFIG     = "FALSE",
  parameter         PLM_AUTO_CONFIG                   = "FALSE",
  parameter         FAST_TRAIN                        = "FALSE",
  parameter         ENABLE_RX_TD_ECRC_TRIM            = "TRUE",
  parameter         DISABLE_SCRAMBLING                = "FALSE",
  parameter   [2:0] PM_CAP_VERSION                    = 3'd3,
  parameter         PM_CAP_PME_CLOCK                  = "FALSE",
  parameter         PM_CAP_DSI                        = "FALSE",
  parameter   [2:0] PM_CAP_AUXCURRENT                 = 3'd0,
  parameter         PM_CAP_D1SUPPORT                  = "TRUE",
  parameter         PM_CAP_D2SUPPORT                  = "TRUE",
  parameter   [4:0] PM_CAP_PMESUPPORT                 = 5'h0F,
  parameter   [7:0] PM_DATA0                          = 8'h00,
  parameter   [1:0] PM_DATA_SCALE0                    = 2'h0,
  parameter   [7:0] PM_DATA1                          = 8'h00,
  parameter   [1:0] PM_DATA_SCALE1                    = 2'h0,
  parameter   [7:0] PM_DATA2                          = 8'h00,
  parameter   [1:0] PM_DATA_SCALE2                    = 2'h0,
  parameter   [7:0] PM_DATA3                          = 8'h00,
  parameter   [1:0] PM_DATA_SCALE3                    = 2'h0,
  parameter   [7:0] PM_DATA4                          = 8'h00,
  parameter   [1:0] PM_DATA_SCALE4                    = 2'h0,
  parameter   [7:0] PM_DATA5                          = 8'h00,
  parameter   [1:0] PM_DATA_SCALE5                    = 2'h0,
  parameter   [7:0] PM_DATA6                          = 8'h00,
  parameter   [1:0] PM_DATA_SCALE6                    = 2'h0,
  parameter   [7:0] PM_DATA7                          = 8'h00,
  parameter   [1:0] PM_DATA_SCALE7                    = 2'h0,
  parameter  [11:0] PCIE_GENERIC                      = 12'b000011101111,
  parameter   [0:0] GTP_SEL                           = 1'b0,
  parameter  [15:0] CFG_VEN_ID                        = 16'h10EE,
  parameter  [15:0] CFG_DEV_ID                        = 16'h1337,
  parameter   [7:0] CFG_REV_ID                        = 8'h00,
  parameter  [15:0] CFG_SUBSYS_VEN_ID                 = 16'h10EE,
  parameter  [15:0] CFG_SUBSYS_ID                     = 16'h0007,
  parameter         REF_CLK_FREQ                      = 1) (
  // PCI Express Fabric Interface
  output            pci_exp_txp,
  output            pci_exp_txn,
  input             pci_exp_rxp,
  input             pci_exp_rxn,

  // Transaction (TRN) Interface
  output            user_lnk_up,

  // Tx

  output            s_axis_tx_tready,
  input  [31:0]     s_axis_tx_tdata,
  input  [3:0]      s_axis_tx_tkeep,
  input  [3:0]      s_axis_tx_tuser,
  input             s_axis_tx_tlast,
  input             s_axis_tx_tvalid,

  output     [5:0]  tx_buf_av,
  output            tx_err_drop,
  input             tx_cfg_gnt,
  output            tx_cfg_req,

  // Rx

  output  [31:0]    m_axis_rx_tdata,
  output  [3:0]     m_axis_rx_tkeep,
  output            m_axis_rx_tlast,
  output            m_axis_rx_tvalid,
  input             m_axis_rx_tready,
  output    [21:0]  m_axis_rx_tuser,
  input             rx_np_ok,

  input       [2:0] fc_sel,
  output      [7:0] fc_nph,
  output     [11:0] fc_npd,
  output      [7:0] fc_ph,
  output     [11:0] fc_pd,
  output      [7:0] fc_cplh,
  output     [11:0] fc_cpld,

  // Host (CFG) Interface
  output     [31:0] cfg_do,
  output            cfg_rd_wr_done,
  input       [9:0] cfg_dwaddr,
  input             cfg_rd_en,
  input             cfg_err_ur,
  input             cfg_err_cor,
  input             cfg_err_ecrc,
  input             cfg_err_cpl_timeout,
  input             cfg_err_cpl_abort,
  input             cfg_err_posted,
  input             cfg_err_locked,
  input      [47:0] cfg_err_tlp_cpl_header,
  output            cfg_err_cpl_rdy,
  input             cfg_interrupt,
  output            cfg_interrupt_rdy,
  input             cfg_interrupt_assert,
  output      [7:0] cfg_interrupt_do,
  input       [7:0] cfg_interrupt_di,
  output      [2:0] cfg_interrupt_mmenable,
  output            cfg_interrupt_msienable,
  input             cfg_turnoff_ok,
  output            cfg_to_turnoff,
  input             cfg_pm_wake,
  output      [2:0] cfg_pcie_link_state,
  input             cfg_trn_pending,
  input      [63:0] cfg_dsn,
  output      [7:0] cfg_bus_number,
  output      [4:0] cfg_device_number,
  output      [2:0] cfg_function_number,
  output     [15:0] cfg_status,
  output     [15:0] cfg_command,
  output     [15:0] cfg_dstatus,
  output     [15:0] cfg_dcommand,
  output     [15:0] cfg_lstatus,
  output     [15:0] cfg_lcommand,

  // System Interface
  input             sys_clk,
  input             sys_reset,
  output            user_clk_out,
  output            user_reset_out,
  output            received_hot_reset
  );

  //***************************************************************************
  // Wire Declarations
  //***************************************************************************

  wire  [31:0]  trn_td;
  wire          trn_tsof;
  wire          trn_teof;
  wire          trn_tsrc_rdy;
  wire          trn_tdst_rdy_n;
  wire          trn_terr_drop_n;
  wire          trn_tsrc_dsc;
  wire          trn_terrfwd;
  wire          trn_tstr;
  wire          trn_tcfg_req_n;
  wire          trn_tcfg_gnt;

  wire  [31:0]  trn_rd;
  wire          trn_rsof_n;
  wire          trn_reof_n;
  wire          trn_rsrc_rdy_n;
  wire          trn_rsrc_dsc_n;
  wire          trn_rdst_rdy;
  wire          trn_rerrfwd_n;
  wire   [6:0]  trn_rbar_hit_n;

  wire          cfg_rd_wr_done_n;
  wire          cfg_interrupt_rdy_n;
  wire          cfg_turnoff_ok_w;
  wire          cfg_to_turnoff_n;
  wire          cfg_err_cpl_rdy_n;
  wire          user_reset_out_w;
  wire          user_lnk_up_w;



  assign        cfg_rd_wr_done          = !cfg_rd_wr_done_n ;
  wire          cfg_rd_en_n             = !cfg_rd_en ;
  wire          cfg_trn_pending_n       = !cfg_trn_pending ;
  wire          cfg_turnoff_ok_n        = !cfg_turnoff_ok_w ;
  wire          cfg_pm_wake_n           = !cfg_pm_wake ;
  wire          cfg_interrupt_n         = !cfg_interrupt ;
  assign        cfg_interrupt_rdy       = !cfg_interrupt_rdy_n ;
  wire          cfg_interrupt_assert_n  = !cfg_interrupt_assert ;
  wire          cfg_err_ecrc_n          = !cfg_err_ecrc ;
  wire          cfg_err_ur_n            = !cfg_err_ur ;
  wire          cfg_err_cpl_timeout_n   = !cfg_err_cpl_timeout ;
  wire          cfg_err_cpl_abort_n     = !cfg_err_cpl_abort ;
  wire          cfg_err_posted_n        = !cfg_err_posted ;
  wire          cfg_err_cor_n           = !cfg_err_cor ;
  assign        cfg_err_cpl_rdy         = !cfg_err_cpl_rdy_n ;
  wire          cfg_err_locked_n        = !cfg_err_locked ;
  assign        tx_err_drop             = !trn_terr_drop_n;
  assign        tx_cfg_req              = !trn_tcfg_req_n;
  assign        cfg_to_turnoff          = !cfg_to_turnoff_n;
  assign        user_reset_out          = !user_reset_out_w;
  wire          sys_reset_n             = !sys_reset;
  assign        user_lnk_up             = !user_lnk_up_w;


  // Wires for the PLL
  wire          mgt_clk;
  wire          mgt_clk_2x;
  wire          clock_locked;
  wire  [1:0]   gt_refclk_out;
  wire          pll_rst;
  wire          clk_125;
  wire          clk_250;
  wire          clk_62_5;
  wire          gt_refclk_buf;
  wire          gt_refclk_fb;

  // These values may be brought out and driven dynamically
  // from pins rather than attributes if desired. Note -
  // if they are not statically driven, the values must be
  // stable before sys_reset_n is released
  wire  [15:0]  w_cfg_ven_id;
  wire  [15:0]  w_cfg_dev_id;
  wire   [7:0]  w_cfg_rev_id;
  wire  [15:0]  w_cfg_subsys_ven_id;
  wire  [15:0]  w_cfg_subsys_id;

  assign w_cfg_ven_id         = CFG_VEN_ID;
  assign w_cfg_dev_id         = CFG_DEV_ID;
  assign w_cfg_rev_id         = CFG_REV_ID;
  assign w_cfg_subsys_ven_id  = CFG_SUBSYS_VEN_ID;
  assign w_cfg_subsys_id      = CFG_SUBSYS_ID;

  wire  [4:0]   cfg_ltssm_state;
  wire  [1:0]   cfg_link_control_aspm_control;
  wire          cfg_link_control_rcb;
  wire          cfg_link_control_common_clock;
  wire          cfg_link_control_extended_sync;
  wire          cfg_command_interrupt_disable;
  wire          cfg_command_serr_en;
  wire          cfg_command_bus_master_enable;
  wire          cfg_command_mem_enable;
  wire          cfg_command_io_enable;
  wire          cfg_dev_status_ur_detected;
  wire          cfg_dev_status_fatal_err_detected;
  wire          cfg_dev_status_nonfatal_err_detected;
  wire          cfg_dev_status_corr_err_detected;
  wire [2:0]    cfg_dev_control_max_read_req;
  wire          cfg_dev_control_no_snoop_en;
  wire          cfg_dev_control_aux_power_en;
  wire          cfg_dev_control_phantom_en;
  wire          cfg_dev_cntrol_ext_tag_en;
  wire [2:0]    cfg_dev_control_max_payload;
  wire          cfg_dev_control_enable_ro;
  wire          cfg_dev_control_ur_err_reporting_en;
  wire          cfg_dev_control_fatal_err_reporting_en;
  wire          cfg_dev_control_non_fatal_reporting_en;
  wire          cfg_dev_control_corr_err_reporting_en;

  wire          mim_rx_rdata_unused;
  wire [11:0]   mim_tx_waddr, mim_tx_raddr, mim_rx_waddr, mim_rx_raddr;
  wire [35:0]   mim_tx_wdata, mim_tx_rdata;
  wire [34:0]   mim_rx_wdata, mim_rx_rdata;
  wire          mim_tx_wen, mim_tx_ren, mim_rx_wen, mim_rx_ren;

  wire          dbg_bad_dllp_status;
  wire          dbg_bad_tlp_lcrc;
  wire          dbg_bad_tlp_seq_num;
  wire          dbg_bad_tlp_status;
  wire          dbg_dl_protocol_status;
  wire          dbg_fc_protocol_err_status;
  wire          dbg_mlfrmd_length;
  wire          dbg_mlfrmd_mps;
  wire          dbg_mlfrmd_tcvc;
  wire          dbg_mlfrmd_tlp_status;
  wire          dbg_mlfrmd_unrec_type;
  wire          dbg_poistlpstatus;
  wire          dbg_rcvr_overflow_status;
  wire          dbg_reg_detected_correctable;
  wire          dbg_reg_detected_fatal;
  wire          dbg_reg_detected_non_fatal;
  wire          dbg_reg_detected_unsupported;
  wire          dbg_rply_rollover_status;
  wire          dbg_rply_timeout_status;
  wire          dbg_ur_no_bar_hit;
  wire          dbg_ur_pois_cfg_wr;
  wire          dbg_ur_status;
  wire          dbg_ur_unsup_msg;

  wire [1:0]    pipe_gt_power_down_a;
  wire [1:0]    pipe_gt_power_down_b;
  wire          pipe_gt_reset_done_a;
  wire          pipe_gt_reset_done_b;
  wire          pipe_gt_tx_elec_idle_a;
  wire          pipe_gt_tx_elec_idle_b;
  wire          pipe_phy_status_a;
  wire          pipe_phy_status_b;
  wire [1:0]    pipe_rx_charisk_a;
  wire [1:0]    pipe_rx_charisk_b;
  wire [15:0]   pipe_rx_data_a;
  wire [15:0]   pipe_rx_data_b;
  wire          pipe_rx_enter_elec_idle_a;
  wire          pipe_rx_enter_elec_idle_b;
  wire          pipe_rx_polarity_a;
  wire          pipe_rx_polarity_b;
  wire          pipe_rxreset_a;
  wire          pipe_rxreset_b;
  wire [2:0]    pipe_rx_status_a;
  wire [2:0]    pipe_rx_status_b;
  wire [1:0]    pipe_tx_char_disp_mode_a;
  wire [1:0]    pipe_tx_char_disp_mode_b;
  wire [1:0]    pipe_tx_char_disp_val_a;
  wire [1:0]    pipe_tx_char_disp_val_b;
  wire [1:0]    pipe_tx_char_is_k_a;
  wire [1:0]    pipe_tx_char_is_k_b;
  wire [15:0]   pipe_tx_data_a;
  wire [15:0]   pipe_tx_data_b;
  wire          pipe_tx_rcvr_det_a;
  wire          pipe_tx_rcvr_det_b;

  // GT->PLM PIPE Interface rx
  wire [1:0]    rx_char_is_k;
  wire [15:0]   rx_data;
  wire          rx_enter_elecidle;
  wire [2:0]    rx_status;
  wire          rx_polarity;

  // GT<-PLM PIPE Interface tx
  wire [1:0]    tx_char_disp_mode;
  wire [1:0]    tx_char_is_k;
  wire          tx_rcvr_det;
  wire [15:0]   tx_data;

  // GT<->PLM PIPE Interface Misc
  wire          phystatus;

  // GT<->PLM PIPE Interface MGT Logic I/O
  wire          gt_reset_done;
  wire          gt_rx_valid;
  wire          gt_tx_elec_idle;
  wire [1:0]    gt_power_down;
  wire          rxreset;
  wire          gt_plllkdet_out;

  // Buffer reference clock from GTP
  BUFIO2 gt_refclk_bufio2 (
    .DIVCLK        ( gt_refclk_buf    ),
    .IOCLK         (                  ),
    .SERDESSTROBE  (                  ),
    .I             ( gt_refclk_out[0] )
  );

  localparam CLKFBOUT_MULT  = (REF_CLK_FREQ == 0) ? 5 : 4;

  localparam CLKIN_PERIOD   = (REF_CLK_FREQ == 0) ? 10 : 8;

  PLL_BASE #(
    //  5 for 100 MHz, 4 for 125Mhz
    .CLKFBOUT_MULT    ( CLKFBOUT_MULT ),
    .CLKFBOUT_PHASE   ( 0             ),
    // 10 for 100 MHz, 8 for 125Mhz
    .CLKIN_PERIOD     ( CLKIN_PERIOD  ),
    .CLKOUT0_DIVIDE   ( 2             ),
    .CLKOUT0_PHASE    ( 0             ),
    .CLKOUT1_DIVIDE   ( 4             ),
    .CLKOUT1_PHASE    ( 0             ),
    .CLKOUT2_DIVIDE   ( 8             ),
    .CLKOUT2_PHASE    ( 0             ),
    .COMPENSATION     ( "INTERNAL"    )
  ) pll_base_i (
    .CLKIN            ( gt_refclk_buf ),
    .CLKFBIN          ( gt_refclk_fb  ),
    .RST              ( pll_rst       ),
    .CLKOUT0          ( clk_250       ),
    .CLKOUT1          ( clk_125       ),
    .CLKOUT2          ( clk_62_5      ),
    .CLKOUT3          (               ),
    .CLKOUT4          (               ),
    .CLKOUT5          (               ),
    .CLKFBOUT         ( gt_refclk_fb  ),
    .LOCKED           ( clock_locked  )
  );

  //******************************************************************//
  // Instantiate buffers where required                               //
  //******************************************************************//
  BUFG  mgt_bufg    (.O(mgt_clk),    .I(clk_125));
  BUFG  mgt2x_bufg  (.O(mgt_clk_2x), .I(clk_250));
  BUFG  phy_bufg    (.O(user_clk_out),    .I(clk_62_5));

  //***************************************************************************
  // PCI Express BRAM Instance
  //***************************************************************************
  pcie_bram_top_s6 #(
    .DEV_CAP_MAX_PAYLOAD_SUPPORTED  ( DEV_CAP_MAX_PAYLOAD_SUPPORTED ),

    .VC0_TX_LASTPACKET              ( VC0_TX_LASTPACKET             ),
    .TLM_TX_OVERHEAD                ( 20                            ),
    .TL_TX_RAM_RADDR_LATENCY        ( TL_TX_RAM_RADDR_LATENCY       ),
    .TL_TX_RAM_RDATA_LATENCY        ( TL_TX_RAM_RDATA_LATENCY       ),
    // NOTE: use the RX value here since there is no separate TX value
    .TL_TX_RAM_WRITE_LATENCY        ( TL_RX_RAM_WRITE_LATENCY       ),

    .VC0_RX_LIMIT                   ( VC0_RX_RAM_LIMIT              ),
    .TL_RX_RAM_RADDR_LATENCY        ( TL_RX_RAM_RADDR_LATENCY       ),
    .TL_RX_RAM_RDATA_LATENCY        ( TL_RX_RAM_RDATA_LATENCY       ),
    .TL_RX_RAM_WRITE_LATENCY        ( TL_RX_RAM_WRITE_LATENCY       )
  ) pcie_bram_top (
    .user_clk_i                     ( user_clk_out                  ),
    .reset_i                        ( user_reset_out                ),

    .mim_tx_waddr                   ( mim_tx_waddr                  ),
    .mim_tx_wen                     ( mim_tx_wen                    ),
    .mim_tx_ren                     ( mim_tx_ren                    ),
    .mim_tx_rce                     ( 1'b1                          ),
    .mim_tx_wdata                   ( mim_tx_wdata                  ),
    .mim_tx_raddr                   ( mim_tx_raddr                  ),
    .mim_tx_rdata                   ( mim_tx_rdata                  ),

    .mim_rx_waddr                   ( mim_rx_waddr                  ),
    .mim_rx_wen                     ( mim_rx_wen                    ),
    .mim_rx_ren                     ( mim_rx_ren                    ),
    .mim_rx_rce                     ( 1'b1                          ),
    .mim_rx_wdata                   ( {1'b0, mim_rx_wdata}          ),
    .mim_rx_raddr                   ( mim_rx_raddr                  ),
    .mim_rx_rdata                   ( {mim_rx_rdata_unused, mim_rx_rdata}     )
  );

  //***************************************************************************
  // PCI Express GTA1_DUAL Wrapper Instance
  //***************************************************************************
  localparam GT_CLK25_DIVIDER  = (REF_CLK_FREQ == 0) ? 4 : 5;

  localparam GT_PLL_DIVSEL_FB  = (REF_CLK_FREQ == 0) ? 5 : 2;

  localparam GT_PLL_DIVSEL_REF = (REF_CLK_FREQ == 0) ? 2 : 1;

  GTPA1_DUAL_WRAPPER #
  (
    // Simulation attributes
    .WRAPPER_SIM_GTPRESET_SPEEDUP (1),
    .WRAPPER_CLK25_DIVIDER_0      (GT_CLK25_DIVIDER),
    .WRAPPER_CLK25_DIVIDER_1      (GT_CLK25_DIVIDER),
    .WRAPPER_PLL_DIVSEL_FB_0      (GT_PLL_DIVSEL_FB),
    .WRAPPER_PLL_DIVSEL_FB_1      (GT_PLL_DIVSEL_FB),
    .WRAPPER_PLL_DIVSEL_REF_0     (GT_PLL_DIVSEL_REF),
    .WRAPPER_PLL_DIVSEL_REF_1     (GT_PLL_DIVSEL_REF),
    .WRAPPER_SIMULATION           (FAST_TRAIN == "TRUE" ? 1 : 0)
  ) GT_i
  (

    //---------------------- Loopback and Powerdown Ports ----------------------
    .TILE0_RXPOWERDOWN0_IN (gt_power_down),
    .TILE0_RXPOWERDOWN1_IN (2'b10),
    .TILE0_TXPOWERDOWN0_IN (gt_power_down),
    .TILE0_TXPOWERDOWN1_IN (2'b10),
    //------------------------------- PLL Ports --------------------------------
    .TILE0_CLK00_IN       (sys_clk),
    .TILE0_CLK01_IN       (1'b0),
    .TILE0_GTPRESET0_IN   (sys_reset),
    .TILE0_GTPRESET1_IN   (1'b1),
    .TILE0_PLLLKDET0_OUT  (gt_plllkdet_out),
    .TILE0_PLLLKDET1_OUT  (),
    .TILE0_RESETDONE0_OUT (gt_reset_done),
    .TILE0_RESETDONE1_OUT (),
    //--------------------- Receive Ports - 8b10b Decoder ----------------------
    .TILE0_RXCHARISK0_OUT    ({rx_char_is_k[0], rx_char_is_k[1]}),
    .TILE0_RXCHARISK1_OUT    (),
    .TILE0_RXDISPERR0_OUT    (),
    .TILE0_RXDISPERR1_OUT    (),
    .TILE0_RXNOTINTABLE0_OUT (),
    .TILE0_RXNOTINTABLE1_OUT (),
    //-------------------- Receive Ports - Clock Correction --------------------
    .TILE0_RXCLKCORCNT0_OUT (),
    .TILE0_RXCLKCORCNT1_OUT (),
    //------------- Receive Ports - Comma Detection and Alignment --------------
    .TILE0_RXENMCOMMAALIGN0_IN (1'b1),
    .TILE0_RXENMCOMMAALIGN1_IN (1'b1),
    .TILE0_RXENPCOMMAALIGN0_IN (1'b1),
    .TILE0_RXENPCOMMAALIGN1_IN (1'b1),
    //----------------- Receive Ports - RX Data Path interface -----------------
    .TILE0_RXDATA0_OUT   ({rx_data[7:0], rx_data[15:8]}),
    .TILE0_RXDATA1_OUT   (),
    .TILE0_RXRESET0_IN   (rxreset),
    .TILE0_RXRESET1_IN   (1'b1),
    .TILE0_RXUSRCLK0_IN  (mgt_clk_2x),
    .TILE0_RXUSRCLK1_IN  (1'b0),
    .TILE0_RXUSRCLK20_IN (mgt_clk),
    .TILE0_RXUSRCLK21_IN (1'b0),
    //----- Receive Ports - RX Driver,OOB signalling,Coupling and Eq.,CDR ------
    .TILE0_GATERXELECIDLE0_IN (1'b0),
    .TILE0_GATERXELECIDLE1_IN (1'b0),
    .TILE0_IGNORESIGDET0_IN   (1'b0),
    .TILE0_IGNORESIGDET1_IN   (1'b0),
    .TILE0_RXELECIDLE0_OUT    (rx_enter_elecidle),
    .TILE0_RXELECIDLE1_OUT    (),
    .TILE0_RXN0_IN            (pci_exp_rxn),
    .TILE0_RXN1_IN            (1'b0),
    .TILE0_RXP0_IN            (pci_exp_rxp),
    .TILE0_RXP1_IN            (1'b0),
    //--------- Receive Ports - RX Elastic Buffer and Phase Alignment ----------
    .TILE0_RXSTATUS0_OUT (rx_status),
    .TILE0_RXSTATUS1_OUT (),
    //------------ Receive Ports - RX Pipe Control for PCI Express -------------
    .TILE0_PHYSTATUS0_OUT (phystatus),
    .TILE0_PHYSTATUS1_OUT (),
    .TILE0_RXVALID0_OUT   (gt_rx_valid),
    .TILE0_RXVALID1_OUT   (),
    //------------------ Receive Ports - RX Polarity Control -------------------
    .TILE0_RXPOLARITY0_IN (rx_polarity),
    .TILE0_RXPOLARITY1_IN (1'b0),
    //-------------------------- TX/RX Datapath Ports --------------------------
    .TILE0_GTPCLKOUT0_OUT (gt_refclk_out),
    .TILE0_GTPCLKOUT1_OUT (),
    //----------------- Transmit Ports - 8b10b Encoder Control -----------------
    .TILE0_TXCHARDISPMODE0_IN ({tx_char_disp_mode[0], tx_char_disp_mode[1]}),
    .TILE0_TXCHARDISPMODE1_IN (2'b00),
    .TILE0_TXCHARISK0_IN      ({tx_char_is_k[0], tx_char_is_k[1]}),
    .TILE0_TXCHARISK1_IN      (2'b00),
    //---------------- Transmit Ports - TX Buffer and Phase Alignment ----------
    //---------------- Transmit Ports - TX Data Path interface -----------------
    .TILE0_TXDATA0_IN    ({tx_data[7:0], tx_data[15:8]}),
    .TILE0_TXDATA1_IN    (16'd0),
    .TILE0_TXUSRCLK0_IN  (mgt_clk_2x),
    .TILE0_TXUSRCLK1_IN  (1'b0),
    .TILE0_TXUSRCLK20_IN (mgt_clk),
    .TILE0_TXUSRCLK21_IN (1'b0),
    //------------- Transmit Ports - TX Driver and OOB signalling --------------
    .TILE0_TXN0_OUT (pci_exp_txn),
    .TILE0_TXN1_OUT (),
    .TILE0_TXP0_OUT (pci_exp_txp),
    .TILE0_TXP1_OUT (),
    //--------------- Transmit Ports - TX Ports for PCI Express ----------------
    .TILE0_TXDETECTRX0_IN (tx_rcvr_det),
    .TILE0_TXDETECTRX1_IN (1'b0),
    .TILE0_TXELECIDLE0_IN (gt_tx_elec_idle),
    .TILE0_TXELECIDLE1_IN (1'b0)
  );

  // Generate the reset for the PLL
  assign pll_rst = !gt_plllkdet_out || !sys_reset_n;


  //***************************************************************************
  // Generate the connection between PCIE_A1 block and the GTPA1_DUAL.  When
  // the parameter GTP_SEL is 0, connect to PIPEA, when it is a 1, connect to
  // PIPEB.
  //***************************************************************************
  generate if (!GTP_SEL) begin : PIPE_A_SEL

    // Signals from GTPA1_DUAL to PCIE_A1
    assign   pipe_rx_charisk_a         = rx_char_is_k;
    assign   pipe_rx_data_a            = rx_data;
    assign   pipe_rx_enter_elec_idle_a = rx_enter_elecidle;
    assign   pipe_rx_status_a          = rx_status;
    assign   pipe_phy_status_a         = phystatus;
    assign   pipe_gt_reset_done_a      = gt_reset_done;

    // Unused PCIE_A1 inputs
    assign   pipe_rx_charisk_b         = 2'b0;
    assign   pipe_rx_data_b            = 16'h0;
    assign   pipe_rx_enter_elec_idle_b = 1'b0;
    assign   pipe_rx_status_b          = 3'b0;
    assign   pipe_phy_status_b         = 1'b0;
    assign   pipe_gt_reset_done_b      = 1'b0;

    //Signals from PCIE_A1 to GTPA1_DUAL
    assign   rx_polarity               = pipe_rx_polarity_a;
    assign   tx_char_disp_mode         = pipe_tx_char_disp_mode_a;
    assign   tx_char_is_k              = pipe_tx_char_is_k_a;
    assign   tx_rcvr_det               = pipe_tx_rcvr_det_a;
    assign   tx_data                   = pipe_tx_data_a;
    assign   gt_tx_elec_idle           = pipe_gt_tx_elec_idle_a;
    assign   gt_power_down             = pipe_gt_power_down_a;
    assign   rxreset                   = pipe_rxreset_a;

  end else begin : PIPE_B_SEL

    // Signals from GTPA1_DUAL to PCIE_A1
    assign   pipe_rx_charisk_b         = rx_char_is_k;
    assign   pipe_rx_data_b            = rx_data;
    assign   pipe_rx_enter_elec_idle_b = rx_enter_elecidle;
    assign   pipe_rx_status_b          = rx_status;
    assign   pipe_phy_status_b         = phystatus;
    assign   pipe_gt_reset_done_b      = gt_reset_done;

    // Unused PCIE_A1 inputs
    assign   pipe_rx_charisk_a         = 2'b0;
    assign   pipe_rx_data_a            = 16'h0;
    assign   pipe_rx_enter_elec_idle_a = 1'b0;
    assign   pipe_rx_status_a          = 3'b0;
    assign   pipe_phy_status_a         = 1'b0;
    assign   pipe_gt_reset_done_a      = 1'b0;

    //Signals from PCIE_A1 to GTPA1_DUAL
    assign   rx_polarity               = pipe_rx_polarity_b;
    assign   tx_char_disp_mode         = pipe_tx_char_disp_mode_b;
    assign   tx_char_is_k              = pipe_tx_char_is_k_b;
    assign   tx_rcvr_det               = pipe_tx_rcvr_det_b;
    assign   tx_data                   = pipe_tx_data_b;
    assign   gt_tx_elec_idle           = pipe_gt_tx_elec_idle_b;
    assign   gt_power_down             = pipe_gt_power_down_b;
    assign   rxreset                   = pipe_rxreset_b;

  end
  endgenerate


  //***************************************************************************
  // AXI Basic Bridge
  //***************************************************************************

  axi_basic_top #(
  .C_DATA_WIDTH     (32),                 // RX/TX interface data width
  .C_FAMILY         ("S6"),               // Targeted FPGA family
  .C_ROOT_PORT      ("FALSE"),            // PCIe block is in root port mode
  .C_PM_PRIORITY    ("TRUE")              // Disable TX packet boundary thrtl

  ) axi_basic_top (
  //---------------------------------------------//
  // User Design I/O                             //
  //---------------------------------------------//

  // AXI TX
  //-----------
  .s_axis_tx_tdata          (s_axis_tx_tdata),    //  input
  .s_axis_tx_tvalid         (s_axis_tx_tvalid),   //  input
  .s_axis_tx_tready         (s_axis_tx_tready),   //  output
  .s_axis_tx_tkeep          (s_axis_tx_tkeep),    //  input
  .s_axis_tx_tlast          (s_axis_tx_tlast),    //  input
  .s_axis_tx_tuser          (s_axis_tx_tuser),    //  input

    // AXI RX
    //-----------
  .m_axis_rx_tdata          (m_axis_rx_tdata),    //  output
  .m_axis_rx_tvalid         (m_axis_rx_tvalid),   //  output
  .m_axis_rx_tready         (m_axis_rx_tready),   //  input
  .m_axis_rx_tkeep          (m_axis_rx_tkeep),    //  output
  .m_axis_rx_tlast          (m_axis_rx_tlast),    //  output
  .m_axis_rx_tuser          (m_axis_rx_tuser),    //  output

    // User Misc.
    //-----------
  .user_turnoff_ok          (cfg_turnoff_ok),     //  input
  .user_tcfg_gnt            (tx_cfg_gnt),         //  input

    //---------------------------------------------//
    // PCIe Block I/O                              //
    //---------------------------------------------//

    // TRN TX
    //-----------
  .trn_td                   (trn_td),             //  output
  .trn_tsof                 (trn_tsof),           //  output
  .trn_teof                 (trn_teof),           //  output
  .trn_tsrc_rdy             (trn_tsrc_rdy),       //  output
  .trn_tdst_rdy             (!trn_tdst_rdy_n),    //  input
  .trn_tsrc_dsc             (trn_tsrc_dsc),       //  output
  .trn_trem                 (),                   //  output
  .trn_terrfwd              (trn_terrfwd),        //  output
  .trn_tstr                 (trn_tstr),           //  output
  .trn_tbuf_av              (tx_buf_av),          //  input
  .trn_tecrc_gen            (),                   //  output

    // TRN RX
    //-----------
  .trn_rd                   (trn_rd),             //  input
  .trn_rsof                 (!trn_rsof_n),        //  input
  .trn_reof                 (!trn_reof_n),        //  input
  .trn_rsrc_rdy             (!trn_rsrc_rdy_n),    //  input
  .trn_rdst_rdy             (trn_rdst_rdy),       //  output
  .trn_rsrc_dsc             (!trn_rsrc_dsc_n),    //  input
  .trn_rrem                 (1'b1),               //  input
  .trn_rerrfwd              (!trn_rerrfwd_n),     //  input
  .trn_rbar_hit             (~trn_rbar_hit_n),    //  input
  .trn_recrc_err            (1'b0),               //  input

  // TRN Misc.
  //-----------
  .trn_tcfg_req             (tx_cfg_req),         //  input
  .trn_tcfg_gnt             (trn_tcfg_gnt),       //  output
  .trn_lnk_up               (user_lnk_up),        //  input

  // Artix/Kintex/Virtex PM
  //-----------
  .cfg_pcie_link_state      (cfg_pcie_link_state),//  input

  // Virtex6 PM
  //-----------
  .cfg_pm_send_pme_to       (1'b0),               //  input  NOT USED FOR EP
  .cfg_pmcsr_powerstate     (2'b00),              //  input
  .trn_rdllp_data           (32'h0),              //    input
  .trn_rdllp_src_rdy        (1'b0),               //    input

  // Power Mgmt for S6/V6
  //-----------
  .cfg_to_turnoff           (cfg_to_turnoff),     //  input
  .cfg_turnoff_ok           (cfg_turnoff_ok_w),   //  output

  // System
  //-----------
  .user_clk                 (user_clk_out),       //  input
  .user_rst                 (user_reset_out),     //  input
  .np_counter               ()                    //  output
  );

  //***************************************************************************
  // PCI Express Hard Block Instance (PCIE_A1)
  //***************************************************************************

  PCIE_A1 #(
    .BAR0                               ( BAR0                                    ),
    .BAR1                               ( BAR1                                    ),
    .BAR2                               ( BAR2                                    ),
    .BAR3                               ( BAR3                                    ),
    .BAR4                               ( BAR4                                    ),
    .BAR5                               ( BAR5                                    ),
    .CARDBUS_CIS_POINTER                ( CARDBUS_CIS_POINTER                     ),
    .CLASS_CODE                         ( CLASS_CODE                              ),
    .DEV_CAP_ENDPOINT_L0S_LATENCY       ( DEV_CAP_ENDPOINT_L0S_LATENCY            ),
    .DEV_CAP_ENDPOINT_L1_LATENCY        ( DEV_CAP_ENDPOINT_L1_LATENCY             ),
    .DEV_CAP_EXT_TAG_SUPPORTED          ( DEV_CAP_EXT_TAG_SUPPORTED               ),
    .DEV_CAP_MAX_PAYLOAD_SUPPORTED      ( DEV_CAP_MAX_PAYLOAD_SUPPORTED           ),
    .DEV_CAP_PHANTOM_FUNCTIONS_SUPPORT  ( DEV_CAP_PHANTOM_FUNCTIONS_SUPPORT       ),
    .DEV_CAP_ROLE_BASED_ERROR           ( DEV_CAP_ROLE_BASED_ERROR                ),
    .DISABLE_BAR_FILTERING              ( DISABLE_BAR_FILTERING                   ),
    .DISABLE_ID_CHECK                   ( DISABLE_ID_CHECK                        ),
    .DISABLE_SCRAMBLING                 ( DISABLE_SCRAMBLING                      ),
    .ENABLE_RX_TD_ECRC_TRIM             ( ENABLE_RX_TD_ECRC_TRIM                  ),
    .EXPANSION_ROM                      ( EXPANSION_ROM                           ),
    .FAST_TRAIN                         ( FAST_TRAIN                              ),
    .GTP_SEL                            ( GTP_SEL                                 ),
    .LINK_CAP_ASPM_SUPPORT              ( LINK_CAP_ASPM_SUPPORT                   ),
    .LINK_CAP_L0S_EXIT_LATENCY          ( LINK_CAP_L0S_EXIT_LATENCY               ),
    .LINK_CAP_L1_EXIT_LATENCY           ( LINK_CAP_L1_EXIT_LATENCY                ),
    .LINK_STATUS_SLOT_CLOCK_CONFIG      ( LINK_STATUS_SLOT_CLOCK_CONFIG           ),
    .LL_ACK_TIMEOUT                     ( LL_ACK_TIMEOUT                          ),
    .LL_ACK_TIMEOUT_EN                  ( LL_ACK_TIMEOUT_EN                       ),
    .LL_REPLAY_TIMEOUT                  ( LL_REPLAY_TIMEOUT                       ),
    .LL_REPLAY_TIMEOUT_EN               ( LL_REPLAY_TIMEOUT_EN                    ),
    .MSI_CAP_MULTIMSG_EXTENSION         ( MSI_CAP_MULTIMSG_EXTENSION              ),
    .MSI_CAP_MULTIMSGCAP                ( MSI_CAP_MULTIMSGCAP                     ),
    .PCIE_CAP_CAPABILITY_VERSION        ( PCIE_CAP_CAPABILITY_VERSION             ),
    .PCIE_CAP_DEVICE_PORT_TYPE          ( PCIE_CAP_DEVICE_PORT_TYPE               ),
    .PCIE_CAP_INT_MSG_NUM               ( PCIE_CAP_INT_MSG_NUM                    ),
    .PCIE_CAP_SLOT_IMPLEMENTED          ( PCIE_CAP_SLOT_IMPLEMENTED               ),
    .PCIE_GENERIC                       ( PCIE_GENERIC                            ),
    .PLM_AUTO_CONFIG                    ( PLM_AUTO_CONFIG                         ),
    .PM_CAP_AUXCURRENT                  ( PM_CAP_AUXCURRENT                       ),
    .PM_CAP_DSI                         ( PM_CAP_DSI                              ),
    .PM_CAP_D1SUPPORT                   ( PM_CAP_D1SUPPORT                        ),
    .PM_CAP_D2SUPPORT                   ( PM_CAP_D2SUPPORT                        ),
    .PM_CAP_PME_CLOCK                   ( PM_CAP_PME_CLOCK                        ),
    .PM_CAP_PMESUPPORT                  ( PM_CAP_PMESUPPORT                       ),
    .PM_CAP_VERSION                     ( PM_CAP_VERSION                          ),
    .PM_DATA_SCALE0                     ( PM_DATA_SCALE0                          ),
    .PM_DATA_SCALE1                     ( PM_DATA_SCALE1                          ),
    .PM_DATA_SCALE2                     ( PM_DATA_SCALE2                          ),
    .PM_DATA_SCALE3                     ( PM_DATA_SCALE3                          ),
    .PM_DATA_SCALE4                     ( PM_DATA_SCALE4                          ),
    .PM_DATA_SCALE5                     ( PM_DATA_SCALE5                          ),
    .PM_DATA_SCALE6                     ( PM_DATA_SCALE6                          ),
    .PM_DATA_SCALE7                     ( PM_DATA_SCALE7                          ),
    .PM_DATA0                           ( PM_DATA0                                ),
    .PM_DATA1                           ( PM_DATA1                                ),
    .PM_DATA2                           ( PM_DATA2                                ),
    .PM_DATA3                           ( PM_DATA3                                ),
    .PM_DATA4                           ( PM_DATA4                                ),
    .PM_DATA5                           ( PM_DATA5                                ),
    .PM_DATA6                           ( PM_DATA6                                ),
    .PM_DATA7                           ( PM_DATA7                                ),
    .SLOT_CAP_ATT_BUTTON_PRESENT        ( SLOT_CAP_ATT_BUTTON_PRESENT             ),
    .SLOT_CAP_ATT_INDICATOR_PRESENT     ( SLOT_CAP_ATT_INDICATOR_PRESENT          ),
    .SLOT_CAP_POWER_INDICATOR_PRESENT   ( SLOT_CAP_POWER_INDICATOR_PRESENT        ),
    .TL_RX_RAM_RADDR_LATENCY            ( TL_RX_RAM_RADDR_LATENCY                 ),
    .TL_RX_RAM_RDATA_LATENCY            ( TL_RX_RAM_RDATA_LATENCY                 ),
    .TL_RX_RAM_WRITE_LATENCY            ( TL_RX_RAM_WRITE_LATENCY                 ),
    .TL_TFC_DISABLE                     ( TL_TFC_DISABLE                          ),
    .TL_TX_CHECKS_DISABLE               ( TL_TX_CHECKS_DISABLE                    ),
    .TL_TX_RAM_RADDR_LATENCY            ( TL_TX_RAM_RADDR_LATENCY                 ),
    .TL_TX_RAM_RDATA_LATENCY            ( TL_TX_RAM_RDATA_LATENCY                 ),
    .USR_CFG                            ( USR_CFG                                 ),
    .USR_EXT_CFG                        ( USR_EXT_CFG                             ),
    .VC0_CPL_INFINITE                   ( VC0_CPL_INFINITE                        ),
    .VC0_RX_RAM_LIMIT                   ( VC0_RX_RAM_LIMIT                        ),
    .VC0_TOTAL_CREDITS_CD               ( VC0_TOTAL_CREDITS_CD                    ),
    .VC0_TOTAL_CREDITS_CH               ( VC0_TOTAL_CREDITS_CH                    ),
    .VC0_TOTAL_CREDITS_NPH              ( VC0_TOTAL_CREDITS_NPH                   ),
    .VC0_TOTAL_CREDITS_PD               ( VC0_TOTAL_CREDITS_PD                    ),
    .VC0_TOTAL_CREDITS_PH               ( VC0_TOTAL_CREDITS_PH                    ),
    .VC0_TX_LASTPACKET                  ( VC0_TX_LASTPACKET                       )
  ) PCIE_A1 (
    .CFGBUSNUMBER                       ( cfg_bus_number                          ),
    .CFGCOMMANDBUSMASTERENABLE          ( cfg_command_bus_master_enable           ),
    .CFGCOMMANDINTERRUPTDISABLE         ( cfg_command_interrupt_disable           ),
    .CFGCOMMANDIOENABLE                 ( cfg_command_io_enable                   ),
    .CFGCOMMANDMEMENABLE                ( cfg_command_mem_enable                  ),
    .CFGCOMMANDSERREN                   ( cfg_command_serr_en                     ),
    .CFGDEVCONTROLAUXPOWEREN            ( cfg_dev_control_aux_power_en            ),
    .CFGDEVCONTROLCORRERRREPORTINGEN    ( cfg_dev_control_corr_err_reporting_en   ),
    .CFGDEVCONTROLENABLERO              ( cfg_dev_control_enable_ro               ),
    .CFGDEVCONTROLEXTTAGEN              ( cfg_dev_control_ext_tag_en              ),
    .CFGDEVCONTROLFATALERRREPORTINGEN   ( cfg_dev_control_fatal_err_reporting_en  ),
    .CFGDEVCONTROLMAXPAYLOAD            ( cfg_dev_control_max_payload             ),
    .CFGDEVCONTROLMAXREADREQ            ( cfg_dev_control_max_read_req            ),
    .CFGDEVCONTROLNONFATALREPORTINGEN   ( cfg_dev_control_non_fatal_reporting_en  ),
    .CFGDEVCONTROLNOSNOOPEN             ( cfg_dev_control_no_snoop_en             ),
    .CFGDEVCONTROLPHANTOMEN             ( cfg_dev_control_phantom_en              ),
    .CFGDEVCONTROLURERRREPORTINGEN      ( cfg_dev_control_ur_err_reporting_en     ),
    .CFGDEVICENUMBER                    ( cfg_device_number                       ),
    .CFGDEVID                           ( w_cfg_dev_id                            ),
    .CFGDEVSTATUSCORRERRDETECTED        ( cfg_dev_status_corr_err_detected        ),
    .CFGDEVSTATUSFATALERRDETECTED       ( cfg_dev_status_fatal_err_detected       ),
    .CFGDEVSTATUSNONFATALERRDETECTED    ( cfg_dev_status_nonfatal_err_detected    ),
    .CFGDEVSTATUSURDETECTED             ( cfg_dev_status_ur_detected              ),
    .CFGDO                              ( cfg_do                                  ),
    .CFGDSN                             ( cfg_dsn                                 ),
    .CFGDWADDR                          ( cfg_dwaddr                              ),
    .CFGERRCORN                         ( cfg_err_cor_n                           ),
    .CFGERRCPLABORTN                    ( cfg_err_cpl_abort_n                     ),
    .CFGERRCPLRDYN                      ( cfg_err_cpl_rdy_n                       ),
    .CFGERRCPLTIMEOUTN                  ( cfg_err_cpl_timeout_n                   ),
    .CFGERRECRCN                        ( cfg_err_ecrc_n                          ),
    .CFGERRLOCKEDN                      ( cfg_err_locked_n                        ),
    .CFGERRPOSTEDN                      ( cfg_err_posted_n                        ),
    .CFGERRTLPCPLHEADER                 ( cfg_err_tlp_cpl_header                  ),
    .CFGERRURN                          ( cfg_err_ur_n                            ),
    .CFGFUNCTIONNUMBER                  ( cfg_function_number                     ),
    .CFGINTERRUPTASSERTN                ( cfg_interrupt_assert_n                  ),
    .CFGINTERRUPTDI                     ( cfg_interrupt_di                        ),
    .CFGINTERRUPTDO                     ( cfg_interrupt_do                        ),
    .CFGINTERRUPTMMENABLE               ( cfg_interrupt_mmenable                  ),
    .CFGINTERRUPTMSIENABLE              ( cfg_interrupt_msienable                 ),
    .CFGINTERRUPTN                      ( cfg_interrupt_n                         ),
    .CFGINTERRUPTRDYN                   ( cfg_interrupt_rdy_n                     ),
    .CFGLINKCONTOLRCB                   ( cfg_link_control_rcb                    ),
    .CFGLINKCONTROLASPMCONTROL          ( cfg_link_control_aspm_control           ),
    .CFGLINKCONTROLCOMMONCLOCK          ( cfg_link_control_common_clock           ),
    .CFGLINKCONTROLEXTENDEDSYNC         ( cfg_link_control_extended_sync          ),
    .CFGLTSSMSTATE                      ( cfg_ltssm_state                         ),
    .CFGPCIELINKSTATEN                  ( cfg_pcie_link_state                     ),
    .CFGPMWAKEN                         ( cfg_pm_wake_n                           ),
    .CFGRDENN                           ( cfg_rd_en_n                             ),
    .CFGRDWRDONEN                       ( cfg_rd_wr_done_n                        ),
    .CFGREVID                           ( w_cfg_rev_id                            ),
    .CFGSUBSYSID                        ( w_cfg_subsys_id                         ),
    .CFGSUBSYSVENID                     ( w_cfg_subsys_ven_id                     ),
    .CFGTOTURNOFFN                      ( cfg_to_turnoff_n                        ),
    .CFGTRNPENDINGN                     ( cfg_trn_pending_n                       ),
    .CFGTURNOFFOKN                      ( cfg_turnoff_ok_n                        ),
    .CFGVENID                           ( w_cfg_ven_id                            ),
    .CLOCKLOCKED                        ( clock_locked                            ),
    .DBGBADDLLPSTATUS                   ( dbg_bad_dllp_status                     ),
    .DBGBADTLPLCRC                      ( dbg_bad_tlp_lcrc                        ),
    .DBGBADTLPSEQNUM                    ( dbg_bad_tlp_seq_num                     ),
    .DBGBADTLPSTATUS                    ( dbg_bad_tlp_status                      ),
    .DBGDLPROTOCOLSTATUS                ( dbg_dl_protocol_status                  ),
    .DBGFCPROTOCOLERRSTATUS             ( dbg_fc_protocol_err_status              ),
    .DBGMLFRMDLENGTH                    ( dbg_mlfrmd_length                       ),
    .DBGMLFRMDMPS                       ( dbg_mlfrmd_mps                          ),
    .DBGMLFRMDTCVC                      ( dbg_mlfrmd_tcvc                         ),
    .DBGMLFRMDTLPSTATUS                 ( dbg_mlfrmd_tlp_status                   ),
    .DBGMLFRMDUNRECTYPE                 ( dbg_mlfrmd_unrec_type                   ),
    .DBGPOISTLPSTATUS                   ( dbg_poistlpstatus                       ),
    .DBGRCVROVERFLOWSTATUS              ( dbg_rcvr_overflow_status                ),
    .DBGREGDETECTEDCORRECTABLE          ( dbg_reg_detected_correctable            ),
    .DBGREGDETECTEDFATAL                ( dbg_reg_detected_fatal                  ),
    .DBGREGDETECTEDNONFATAL             ( dbg_reg_detected_non_fatal              ),
    .DBGREGDETECTEDUNSUPPORTED          ( dbg_reg_detected_unsupported            ),
    .DBGRPLYROLLOVERSTATUS              ( dbg_rply_rollover_status                ),
    .DBGRPLYTIMEOUTSTATUS               ( dbg_rply_timeout_status                 ),
    .DBGURNOBARHIT                      ( dbg_ur_no_bar_hit                       ),
    .DBGURPOISCFGWR                     ( dbg_ur_pois_cfg_wr                      ),
    .DBGURSTATUS                        ( dbg_ur_status                           ),
    .DBGURUNSUPMSG                      ( dbg_ur_unsup_msg                        ),
    .MGTCLK                             ( mgt_clk                                 ),
    .MIMRXRADDR                         ( mim_rx_raddr                            ),
    .MIMRXRDATA                         ( mim_rx_rdata                            ),
    .MIMRXREN                           ( mim_rx_ren                              ),
    .MIMRXWADDR                         ( mim_rx_waddr                            ),
    .MIMRXWDATA                         ( mim_rx_wdata                            ),
    .MIMRXWEN                           ( mim_rx_wen                              ),
    .MIMTXRADDR                         ( mim_tx_raddr                            ),
    .MIMTXRDATA                         ( mim_tx_rdata                            ),
    .MIMTXREN                           ( mim_tx_ren                              ),
    .MIMTXWADDR                         ( mim_tx_waddr                            ),
    .MIMTXWDATA                         ( mim_tx_wdata                            ),
    .MIMTXWEN                           ( mim_tx_wen                              ),
    .PIPEGTPOWERDOWNA                   ( pipe_gt_power_down_a                    ),
    .PIPEGTPOWERDOWNB                   ( pipe_gt_power_down_b                    ),
    .PIPEGTRESETDONEA                   ( pipe_gt_reset_done_a                    ),
    .PIPEGTRESETDONEB                   ( pipe_gt_reset_done_b                    ),
    .PIPEGTTXELECIDLEA                  ( pipe_gt_tx_elec_idle_a                  ),
    .PIPEGTTXELECIDLEB                  ( pipe_gt_tx_elec_idle_b                  ),
    .PIPEPHYSTATUSA                     ( pipe_phy_status_a                       ),
    .PIPEPHYSTATUSB                     ( pipe_phy_status_b                       ),
    .PIPERXCHARISKA                     ( pipe_rx_charisk_a                       ),
    .PIPERXCHARISKB                     ( pipe_rx_charisk_b                       ),
    .PIPERXDATAA                        ( pipe_rx_data_a                          ),
    .PIPERXDATAB                        ( pipe_rx_data_b                          ),
    .PIPERXENTERELECIDLEA               ( pipe_rx_enter_elec_idle_a               ),
    .PIPERXENTERELECIDLEB               ( pipe_rx_enter_elec_idle_b               ),
    .PIPERXPOLARITYA                    ( pipe_rx_polarity_a                      ),
    .PIPERXPOLARITYB                    ( pipe_rx_polarity_b                      ),
    .PIPERXRESETA                       ( pipe_rxreset_a                          ),
    .PIPERXRESETB                       ( pipe_rxreset_b                          ),
    .PIPERXSTATUSA                      ( pipe_rx_status_a                        ),
    .PIPERXSTATUSB                      ( pipe_rx_status_b                        ),
    .PIPETXCHARDISPMODEA                ( pipe_tx_char_disp_mode_a                ),
    .PIPETXCHARDISPMODEB                ( pipe_tx_char_disp_mode_b                ),
    .PIPETXCHARDISPVALA                 ( pipe_tx_char_disp_val_a                 ),
    .PIPETXCHARDISPVALB                 ( pipe_tx_char_disp_val_b                 ),
    .PIPETXCHARISKA                     ( pipe_tx_char_is_k_a                     ),
    .PIPETXCHARISKB                     ( pipe_tx_char_is_k_b                     ),
    .PIPETXDATAA                        ( pipe_tx_data_a                          ),
    .PIPETXDATAB                        ( pipe_tx_data_b                          ),
    .PIPETXRCVRDETA                     ( pipe_tx_rcvr_det_a                      ),
    .PIPETXRCVRDETB                     ( pipe_tx_rcvr_det_b                      ),
    .RECEIVEDHOTRESET                   ( received_hot_reset                      ),
    .SYSRESETN                          ( sys_reset_n                             ),
    .TRNFCCPLD                          ( fc_cpld                                 ),
    .TRNFCCPLH                          ( fc_cplh                                 ),
    .TRNFCNPD                           ( fc_npd                                  ),
    .TRNFCNPH                           ( fc_nph                                  ),
    .TRNFCPD                            ( fc_pd                                   ),
    .TRNFCPH                            ( fc_ph                                   ),
    .TRNFCSEL                           ( fc_sel                                  ),
    .TRNLNKUPN                          ( user_lnk_up_w                           ),
    .TRNRBARHITN                        ( trn_rbar_hit_n                          ),
    .TRNRD                              ( trn_rd                                  ),
    .TRNRDSTRDYN                        ( !trn_rdst_rdy                           ),
    .TRNREOFN                           ( trn_reof_n                              ),
    .TRNRERRFWDN                        ( trn_rerrfwd_n                           ),
    .TRNRNPOKN                          ( !rx_np_ok                               ),
    .TRNRSOFN                           ( trn_rsof_n                              ),
    .TRNRSRCDSCN                        ( trn_rsrc_dsc_n                          ),
    .TRNRSRCRDYN                        ( trn_rsrc_rdy_n                          ),
    .TRNTBUFAV                          ( tx_buf_av                               ),
    .TRNTCFGGNTN                        ( !trn_tcfg_gnt                           ),
    .TRNTCFGREQN                        ( trn_tcfg_req_n                          ),
    .TRNTD                              ( trn_td                                  ),
    .TRNTDSTRDYN                        ( trn_tdst_rdy_n                          ),
    .TRNTEOFN                           ( !trn_teof                               ),
    .TRNTERRDROPN                       ( trn_terr_drop_n                         ),
    .TRNTERRFWDN                        ( !trn_terrfwd                            ),
    .TRNTSOFN                           ( !trn_tsof                               ),
    .TRNTSRCDSCN                        ( !trn_tsrc_dsc                           ),
    .TRNTSRCRDYN                        ( !trn_tsrc_rdy                           ),
    .TRNTSTRN                           ( !trn_tstr                               ),
    .USERCLK                            ( user_clk_out                            ),
    .USERRSTN                           ( user_reset_out_w                        )
  );

  //***************************************************************************
  // Recreate wrapper outputs from the PCIE_A1 signals.
  //***************************************************************************
  assign      cfg_status  = {16'b0};

  assign      cfg_command = {5'b0,
                             cfg_command_interrupt_disable,
                             1'b0,
                             cfg_command_serr_en,
                             5'b0,
                             cfg_command_bus_master_enable,
                             cfg_command_mem_enable,
                             cfg_command_io_enable};

  assign      cfg_dstatus  = {10'h0,
                             !cfg_trn_pending_n,
                             1'b0,
                             cfg_dev_status_ur_detected,
                             cfg_dev_status_fatal_err_detected,
                             cfg_dev_status_nonfatal_err_detected,
                             cfg_dev_status_corr_err_detected};

  assign      cfg_dcommand = {1'b0,
                             cfg_dev_control_max_read_req,
                             cfg_dev_control_no_snoop_en,
                             cfg_dev_control_aux_power_en,
                             cfg_dev_control_phantom_en,
                             cfg_dev_control_ext_tag_en,
                             cfg_dev_control_max_payload,
                             cfg_dev_control_enable_ro,
                             cfg_dev_control_ur_err_reporting_en,
                             cfg_dev_control_fatal_err_reporting_en,
                             cfg_dev_control_non_fatal_reporting_en,
                             cfg_dev_control_corr_err_reporting_en};

  assign      cfg_lstatus   = 16'h0011;

  assign      cfg_lcommand  = {8'h0,
                              cfg_link_control_extended_sync,
                              cfg_link_control_common_clock,
                              2'b00,
                              cfg_link_control_rcb,
                              1'b0,
                              cfg_link_control_aspm_control};



endmodule
