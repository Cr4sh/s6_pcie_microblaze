//-----------------------------------------------------------------------------
// microblaze_top.v
//-----------------------------------------------------------------------------

module microblaze_top
  (
    zio,
    rzq,
    mcbx_dram_we_n,
    mcbx_dram_udqs_n,
    mcbx_dram_udqs,
    mcbx_dram_udm,
    mcbx_dram_ras_n,
    mcbx_dram_odt,
    mcbx_dram_ldm,
    mcbx_dram_dqs_n,
    mcbx_dram_dqs,
    mcbx_dram_dq,
    mcbx_dram_ddr3_rst,
    mcbx_dram_clk_n,
    mcbx_dram_clk,
    mcbx_dram_cke,
    mcbx_dram_cas_n,
    mcbx_dram_ba,
    mcbx_dram_addr,
    RS232_Uart_1_sout,
    RS232_Uart_1_sin,
    RESET,
    Push_Buttons_4Bits_TRI_I,
    Linear_Flash_we_n,
    Linear_Flash_reset,
    Linear_Flash_oe_n,
    Linear_Flash_data,
    Linear_Flash_ce_n,
    Linear_Flash_adv_n,
    Linear_Flash_address,
    Ethernet_Lite_TX_EN,
    Ethernet_Lite_TX_CLK,
    Ethernet_Lite_TXD,
    Ethernet_Lite_RX_ER,
    Ethernet_Lite_RX_DV,
    Ethernet_Lite_RX_CLK,
    Ethernet_Lite_RXD,
    Ethernet_Lite_PHY_RST_N,
    Ethernet_Lite_MDIO,
    Ethernet_Lite_MDC,
    Ethernet_Lite_CRS,
    Ethernet_Lite_COL,
    DIP_Switches_4Bits_TRI_I,
    CLK_P,
    CLK_N,
    LEDs_4Bits,
    pcie_clk_n,
    pcie_clk_p,
    pcie_txp,
    pcie_txn,
    pcie_rxp,
    pcie_rxn
  );
  inout zio;
  inout rzq;
  output mcbx_dram_we_n;
  inout mcbx_dram_udqs_n;
  inout mcbx_dram_udqs;
  output mcbx_dram_udm;
  output mcbx_dram_ras_n;
  output mcbx_dram_odt;
  output mcbx_dram_ldm;
  inout mcbx_dram_dqs_n;
  inout mcbx_dram_dqs;
  inout [15:0] mcbx_dram_dq;
  output mcbx_dram_ddr3_rst;
  output mcbx_dram_clk_n;
  output mcbx_dram_clk;
  output mcbx_dram_cke;
  output mcbx_dram_cas_n;
  output [2:0] mcbx_dram_ba;
  output [12:0] mcbx_dram_addr;
  output RS232_Uart_1_sout;
  input RS232_Uart_1_sin;
  input RESET;
  input [3:0] Push_Buttons_4Bits_TRI_I;
  output Linear_Flash_we_n;
  output Linear_Flash_reset;
  output Linear_Flash_oe_n;
  inout [0:15] Linear_Flash_data;
  output Linear_Flash_ce_n;
  output Linear_Flash_adv_n;
  output [0:23] Linear_Flash_address;
  output Ethernet_Lite_TX_EN;
  (* BUFFER_TYPE = "IBUF" *)
  input Ethernet_Lite_TX_CLK;
  output [3:0] Ethernet_Lite_TXD;
  input Ethernet_Lite_RX_ER;
  input Ethernet_Lite_RX_DV;
  (* BUFFER_TYPE = "IBUF" *)
  input Ethernet_Lite_RX_CLK;
  input [3:0] Ethernet_Lite_RXD;
  output Ethernet_Lite_PHY_RST_N;
  inout Ethernet_Lite_MDIO;
  output Ethernet_Lite_MDC;
  input Ethernet_Lite_CRS;
  input Ethernet_Lite_COL;
  input [3:0] DIP_Switches_4Bits_TRI_I;
  input CLK_P;
  input CLK_N;
  output [3:0] LEDs_4Bits;
  input pcie_clk_n;
  input pcie_clk_p;
  output pcie_txp;
  output pcie_txn;
  input pcie_rxp;
  input pcie_rxn;

  (* BOX_TYPE = "user_black_box" *)
  microblaze
    microblaze_i (
      .zio ( zio ),
      .rzq ( rzq ),
      .mcbx_dram_we_n ( mcbx_dram_we_n ),
      .mcbx_dram_udqs_n ( mcbx_dram_udqs_n ),
      .mcbx_dram_udqs ( mcbx_dram_udqs ),
      .mcbx_dram_udm ( mcbx_dram_udm ),
      .mcbx_dram_ras_n ( mcbx_dram_ras_n ),
      .mcbx_dram_odt ( mcbx_dram_odt ),
      .mcbx_dram_ldm ( mcbx_dram_ldm ),
      .mcbx_dram_dqs_n ( mcbx_dram_dqs_n ),
      .mcbx_dram_dqs ( mcbx_dram_dqs ),
      .mcbx_dram_dq ( mcbx_dram_dq ),
      .mcbx_dram_ddr3_rst ( mcbx_dram_ddr3_rst ),
      .mcbx_dram_clk_n ( mcbx_dram_clk_n ),
      .mcbx_dram_clk ( mcbx_dram_clk ),
      .mcbx_dram_cke ( mcbx_dram_cke ),
      .mcbx_dram_cas_n ( mcbx_dram_cas_n ),
      .mcbx_dram_ba ( mcbx_dram_ba ),
      .mcbx_dram_addr ( mcbx_dram_addr ),
      .RS232_Uart_1_sout ( RS232_Uart_1_sout ),
      .RS232_Uart_1_sin ( RS232_Uart_1_sin ),
      .RESET ( RESET ),
      .Push_Buttons_4Bits_TRI_I ( Push_Buttons_4Bits_TRI_I ),
      .Linear_Flash_we_n ( Linear_Flash_we_n ),
      .Linear_Flash_reset ( Linear_Flash_reset ),
      .Linear_Flash_oe_n ( Linear_Flash_oe_n ),
      .Linear_Flash_data ( Linear_Flash_data ),
      .Linear_Flash_ce_n ( Linear_Flash_ce_n ),
      .Linear_Flash_adv_n ( Linear_Flash_adv_n ),
      .Linear_Flash_address ( Linear_Flash_address ),
      .Ethernet_Lite_TX_EN ( Ethernet_Lite_TX_EN ),
      .Ethernet_Lite_TX_CLK ( Ethernet_Lite_TX_CLK ),
      .Ethernet_Lite_TXD ( Ethernet_Lite_TXD ),
      .Ethernet_Lite_RX_ER ( Ethernet_Lite_RX_ER ),
      .Ethernet_Lite_RX_DV ( Ethernet_Lite_RX_DV ),
      .Ethernet_Lite_RX_CLK ( Ethernet_Lite_RX_CLK ),
      .Ethernet_Lite_RXD ( Ethernet_Lite_RXD ),
      .Ethernet_Lite_PHY_RST_N ( Ethernet_Lite_PHY_RST_N ),
      .Ethernet_Lite_MDIO ( Ethernet_Lite_MDIO ),
      .Ethernet_Lite_MDC ( Ethernet_Lite_MDC ),
      .Ethernet_Lite_CRS ( Ethernet_Lite_CRS ),
      .Ethernet_Lite_COL ( Ethernet_Lite_COL ),
      .DIP_Switches_4Bits_TRI_I ( DIP_Switches_4Bits_TRI_I ),
      .CLK_P ( CLK_P ),
      .CLK_N ( CLK_N ),
      .LEDs_4Bits ( LEDs_4Bits ),
      .pcie_clk_n ( pcie_clk_n ),
      .pcie_clk_p ( pcie_clk_p ),
      .pcie_txp ( pcie_txp ),
      .pcie_txn ( pcie_txn ),
      .pcie_rxp ( pcie_rxp ),
      .pcie_rxn ( pcie_rxn )
    );

endmodule

