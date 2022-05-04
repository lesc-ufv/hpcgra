

module m_cgra
(
  input clk,
  input [9-1:0] conf_bus,
  input [17-1:0] in_stream0_0,
  input [17-1:0] in_stream2_0,
  output [17-1:0] out_stream1_0,
  output [17-1:0] out_stream3_0
);

  wire [17-1:0] pe0_to_pe1;
  wire [17-1:0] pe0_to_pe2;
  wire [17-1:0] pe1_to_pe0;
  wire [17-1:0] pe1_to_pe3;
  wire [17-1:0] pe2_to_pe0;
  wire [17-1:0] pe2_to_pe3;
  wire [17-1:0] pe3_to_pe1;
  wire [17-1:0] pe3_to_pe2;
  wire [9-1:0] conf_bus_reg_in [0:4-1];
  wire [9-1:0] conf_bus_reg_out [0:4-1];

  reg_pipe
  #(
    .num_register(4),
    .width(9)
  )
  reg_pipe_conf_0
  (
    .clk(clk),
    .rst(1'b0),
    .en(1'b1),
    .in(conf_bus_reg_in[0]),
    .out(conf_bus_reg_out[0])
  );


  reg_pipe
  #(
    .num_register(4),
    .width(9)
  )
  reg_pipe_conf_1
  (
    .clk(clk),
    .rst(1'b0),
    .en(1'b1),
    .in(conf_bus_reg_in[1]),
    .out(conf_bus_reg_out[1])
  );


  reg_pipe
  #(
    .num_register(4),
    .width(9)
  )
  reg_pipe_conf_2
  (
    .clk(clk),
    .rst(1'b0),
    .en(1'b1),
    .in(conf_bus_reg_in[2]),
    .out(conf_bus_reg_out[2])
  );


  reg_pipe
  #(
    .num_register(4),
    .width(9)
  )
  reg_pipe_conf_3
  (
    .clk(clk),
    .rst(1'b0),
    .en(1'b1),
    .in(conf_bus_reg_in[3]),
    .out(conf_bus_reg_out[3])
  );


  pei1o0n2r4e440alumyaddconstsub_m
  #(
    .id(1),
    .conf_raw_bits(24)
  )
  pe_0
  (
    .clk(clk),
    .conf_bus(conf_bus_reg_out[0]),
    .stream_in0(in_stream0_0),
    .in0(pe1_to_pe0),
    .in1(pe2_to_pe0),
    .out0(pe0_to_pe1),
    .out1(pe0_to_pe2)
  );


  pei0o1n2r4e440aluadd_msub_m
  #(
    .id(2),
    .conf_raw_bits(24)
  )
  pe_1
  (
    .clk(clk),
    .conf_bus(conf_bus_reg_out[1]),
    .stream_out0(out_stream1_0),
    .in0(pe0_to_pe1),
    .in1(pe3_to_pe1),
    .out0(pe1_to_pe0),
    .out1(pe1_to_pe3)
  );


  pei1o0n2r4e440aluadd_msub_m
  #(
    .id(3),
    .conf_raw_bits(24)
  )
  pe_2
  (
    .clk(clk),
    .conf_bus(conf_bus_reg_out[2]),
    .stream_in0(in_stream2_0),
    .in0(pe0_to_pe2),
    .in1(pe3_to_pe2),
    .out0(pe2_to_pe0),
    .out1(pe2_to_pe3)
  );


  pei0o1n2r4e440aluadd_msub_m
  #(
    .id(4),
    .conf_raw_bits(24)
  )
  pe_3
  (
    .clk(clk),
    .conf_bus(conf_bus_reg_out[3]),
    .stream_out0(out_stream3_0),
    .in0(pe1_to_pe3),
    .in1(pe2_to_pe3),
    .out0(pe3_to_pe1),
    .out1(pe3_to_pe2)
  );

  assign conf_bus_reg_in[0] = conf_bus;
  assign conf_bus_reg_in[1] = conf_bus_reg_out[0];
  assign conf_bus_reg_in[2] = conf_bus_reg_out[0];
  assign conf_bus_reg_in[3] = conf_bus_reg_out[1];

endmodule



module reg_pipe #
(
  parameter num_register = 1,
  parameter width = 16
)
(
  input clk,
  input en,
  input rst,
  input [width-1:0] in,
  output [width-1:0] out
);

  reg [width-1:0] regs [0:num_register-1];
  integer i;

  assign out = regs[num_register - 1];

  always @(posedge clk) begin
    if(rst) begin
      regs[0] <= 0;
    end else begin
      if(en) begin
        regs[0] <= in;
        for(i=1; i<num_register; i=i+1) begin
          regs[i] <= regs[i - 1];
        end
      end 
    end
  end

  integer i_initial;

  initial begin
    for(i_initial=0; i_initial<num_register; i_initial=i_initial+1) begin
      regs[i_initial] = 0;
    end
  end


endmodule



module pei1o0n2r4e440alumyaddconstsub_m #
(
  parameter id = 0,
  parameter conf_raw_bits = 0
)
(
  input clk,
  input [9-1:0] conf_bus,
  input [17-1:0] in0,
  input [17-1:0] in1,
  output [17-1:0] out0,
  output [17-1:0] out1,
  input [17-1:0] stream_in0
);

  wire [17-1:0] in_reg0;
  wire [17-1:0] in_reg1;
  wire [17-1:0] router_out0;
  wire [17-1:0] router_out1;
  wire [17-1:0] stream_in_reg0;
  wire reset;
  wire [3*17-1:0] pe_const;
  wire [17-1:0] in_reg_router0;
  wire [17-1:0] in_reg_router1;
  wire [17-1:0] mux_alu_out0;
  wire [17-1:0] mux_alu_out1;
  wire [17-1:0] alu_out0;
  reg [1-1:0] sel_alu_opcode;
  reg [2-1:0] sel_mux_alu0;
  reg [2-1:0] sel_mux_alu1;
  reg [4-1:0] route_sel_in;

  reg_pipe
  #(
    .num_register(1),
    .width(17)
  )
  m_stream_in_reg0
  (
    .clk(clk),
    .rst(1'b0),
    .en(1'b1),
    .in(stream_in0),
    .out(stream_in_reg0)
  );


  reg_pipe
  #(
    .num_register(1),
    .width(17)
  )
  in0_reg
  (
    .clk(clk),
    .rst(1'b0),
    .en(1'b1),
    .in(in0),
    .out(in_reg0)
  );


  reg_pipe
  #(
    .num_register(1),
    .width(17)
  )
  in1_reg
  (
    .clk(clk),
    .rst(1'b0),
    .en(1'b1),
    .in(in1),
    .out(in_reg1)
  );


  multiplexer_4
  #(
    .width(17)
  )
  mux_alu_in0
  (
    .sel(sel_mux_alu0),
    .in0(stream_in_reg0),
    .in1(pe_const[1*17-1:0*17]),
    .in2(in_reg0),
    .in3(in_reg1),
    .out(mux_alu_out0)
  );

  wire [17-1:0] elastic_pipeline_to_alu0;
  reg [3-1:0] sel_elastic_pipeline0;

  elastic_pipeline_6_4
  #(
    .width(17)
  )
  elastic_pipeline0
  (
    .in(mux_alu_out0),
    .out(elastic_pipeline_to_alu0),
    .clk(clk),
    .en(1'b1),
    .latency(sel_elastic_pipeline0)
  );


  multiplexer_4
  #(
    .width(17)
  )
  mux_alu_in1
  (
    .sel(sel_mux_alu1),
    .in0(stream_in_reg0),
    .in1(pe_const[2*17-1:1*17]),
    .in2(in_reg0),
    .in3(in_reg1),
    .out(mux_alu_out1)
  );

  wire [17-1:0] elastic_pipeline_to_alu1;
  reg [3-1:0] sel_elastic_pipeline1;

  elastic_pipeline_6_4
  #(
    .width(17)
  )
  elastic_pipeline1
  (
    .in(mux_alu_out1),
    .out(elastic_pipeline_to_alu1),
    .clk(clk),
    .en(1'b1),
    .latency(sel_elastic_pipeline1)
  );


  alumyaddconstsub_m
  #(
    .width(16)
  )
  alu
  (
    .clk(clk),
    .rst(reset),
    .opcode(sel_alu_opcode),
    .in0(elastic_pipeline_to_alu0),
    .in1(elastic_pipeline_to_alu1),
    .out0(alu_out0),
    .const0(pe_const[3*17-1:2*17])
  );


  reg_pipe
  #(
    .num_register(4),
    .width(17)
  )
  in_reg0_router
  (
    .clk(clk),
    .rst(1'b0),
    .en(1'b1),
    .in(in_reg0),
    .out(in_reg_router0)
  );


  reg_pipe
  #(
    .num_register(4),
    .width(17)
  )
  in_reg1_router
  (
    .clk(clk),
    .rst(1'b0),
    .en(1'b1),
    .in(in_reg1),
    .out(in_reg_router1)
  );


  route_4_3x2
  #(
    .width(17)
  )
  router
  (
    .sel_in(route_sel_in),
    .in0(alu_out0),
    .in1(in_reg_router0),
    .in2(in_reg_router1),
    .out0(router_out0),
    .out1(router_out1)
  );

  wire [11-1:0] conf_alu;
  wire [4-1:0] conf_router;

  pe_conf_reader_alu_in_3_alu_w_11_router_w_4
  #(
    .pe_id(id),
    .conf_raw_bits(conf_raw_bits)
  )
  pe_conf_reader
  (
    .clk(clk),
    .conf_bus(conf_bus),
    .reset(reset),
    .conf_alu(conf_alu),
    .conf_const(pe_const),
    .conf_router(conf_router)
  );


  reg_pipe
  #(
    .num_register(1),
    .width(17)
  )
  out0_reg
  (
    .clk(clk),
    .rst(1'b0),
    .en(1'b1),
    .in(router_out0),
    .out(out0)
  );


  reg_pipe
  #(
    .num_register(1),
    .width(17)
  )
  out1_reg
  (
    .clk(clk),
    .rst(1'b0),
    .en(1'b1),
    .in(router_out1),
    .out(out1)
  );


  always @(posedge clk) begin
    sel_alu_opcode <= conf_alu[0:0];
    sel_mux_alu0 <= conf_alu[2:1];
    sel_mux_alu1 <= conf_alu[4:3];
    sel_elastic_pipeline0 <= conf_alu[7:5];
    sel_elastic_pipeline1 <= conf_alu[10:8];
    route_sel_in <= conf_router[3:0];
  end


  initial begin
    sel_alu_opcode = 0;
    sel_mux_alu0 = 0;
    sel_mux_alu1 = 0;
    route_sel_in = 0;
    sel_elastic_pipeline0 = 0;
    sel_elastic_pipeline1 = 0;
  end


endmodule



module multiplexer_4 #
(
  parameter width = 8
)
(
  input [2-1:0] sel,
  input [width-1:0] in0,
  input [width-1:0] in1,
  input [width-1:0] in2,
  input [width-1:0] in3,
  output [width-1:0] out
);

  wire [width-1:0] aux [0:4-1];
  assign aux[0] = in0;
  assign aux[1] = in1;
  assign aux[2] = in2;
  assign aux[3] = in3;
  assign out = aux[sel];

endmodule



module elastic_pipeline_6_4 #
(
  parameter width = 8
)
(
  input clk,
  input en,
  input [3-1:0] latency,
  input [width-1:0] in,
  output [width-1:0] out
);

  reg [width-1:0] shift_reg [0:24-1];
  integer i;

  always @(posedge clk) begin
    if(en) begin
      shift_reg[0] <= in;
      for(i=1; i<24; i=i+1) begin
        shift_reg[i] <= shift_reg[i - 1];
      end
    end 
  end


  multiplexer_5
  #(
    .width(width)
  )
  mux
  (
    .sel(latency),
    .in0(in),
    .in1(shift_reg[5]),
    .in2(shift_reg[11]),
    .in3(shift_reg[17]),
    .in4(shift_reg[23]),
    .out(out)
  );

  integer i_initial;

  initial begin
    for(i_initial=0; i_initial<24; i_initial=i_initial+1) begin
      shift_reg[i_initial] = 0;
    end
  end


endmodule



module multiplexer_5 #
(
  parameter width = 8
)
(
  input [3-1:0] sel,
  input [width-1:0] in0,
  input [width-1:0] in1,
  input [width-1:0] in2,
  input [width-1:0] in3,
  input [width-1:0] in4,
  output [width-1:0] out
);

  wire [width-1:0] aux [0:5-1];
  assign aux[0] = in0;
  assign aux[1] = in1;
  assign aux[2] = in2;
  assign aux[3] = in3;
  assign aux[4] = in4;
  assign out = aux[sel];

endmodule



module alumyaddconstsub_m #
(
  parameter width = 8
)
(
  input clk,
  input rst,
  input [1-1:0] opcode,
  input [width+1-1:0] in0,
  input [width+1-1:0] in1,
  output [width+1-1:0] out0,
  input [width+1-1:0] const0
);

  reg [width+1-1:0] in_reg0;
  reg [width+1-1:0] in_reg1;
  wire [width+1-1:0] out_ops0 [0:2-1];
  wire [width+1-1:0] out_ops_reg0 [0:2-1];
  reg [width+1-1:0] const0_reg;

  myaddconst
  #(
    .width(width)
  )
  myaddconst
  (
    .clk(clk),
    .rst(rst),
    .in0(in_reg0[width-1:0]),
    .in0_valid(in_reg0[width]),
    .in1(in_reg1[width-1:0]),
    .in1_valid(in_reg1[width]),
    .reg2__out1(out_ops0[0][width-1:0]),
    .reg2__out1_valid(out_ops0[0][width]),
    .myconst__in_const0(const0_reg[width-1:0]),
    .myconst__in_const0_valid(const0_reg[width])
  );

  assign out_ops_reg0[0] = out_ops0[0];

  sub_m
  #(
    .width(width)
  )
  sub_m
  (
    .clk(clk),
    .rst(rst),
    .in0(in_reg0[width-1:0]),
    .in0_valid(in_reg0[width]),
    .in1(in_reg1[width-1:0]),
    .in1_valid(in_reg1[width]),
    .out0(out_ops0[1][width-1:0]),
    .out0_valid(out_ops0[1][width])
  );


  reg_pipe
  #(
    .num_register(3),
    .width(width + 1)
  )
  sub_m_outreg
  (
    .clk(clk),
    .rst(1'b0),
    .en(1'b1),
    .in(out_ops0[1]),
    .out(out_ops_reg0[1])
  );


  always @(posedge clk) begin
    in_reg0 <= in0;
    in_reg1 <= in1;
    const0_reg <= const0;
  end

  assign out0 = out_ops_reg0[opcode];

  initial begin
    in_reg0 = 0;
    in_reg1 = 0;
    const0_reg = 0;
  end


endmodule



module myaddconst #
(
  parameter width = 8
)
(
  input clk,
  input rst,
  input [width-1:0] in0,
  input in0_valid,
  input [width-1:0] in1,
  input in1_valid,
  output [width-1:0] reg2__out1,
  output reg2__out1_valid,
  input [width-1:0] myconst__in_const0,
  input myconst__in_const0_valid
);

  wire [width-1:0] myconst__out1;
  wire myconst__out1_valid;
  wire [width-1:0] add3__out1;
  wire add3__out1_valid;

  myadd
  #(
    .width(width)
  )
  add3
  (
    .clk(clk),
    .rst(rst),
    .in0(in0),
    .in0_valid(in0_valid),
    .in1(in1),
    .in1_valid(in1_valid),
    .in2(myconst__out1),
    .in2_valid(myconst__out1_valid),
    .reg1__out1(add3__out1),
    .reg1__out1_valid(add3__out1_valid)
  );


  const_m
  #(
    .width(width)
  )
  myconst
  (
    .clk(clk),
    .rst(rst),
    .in0(myconst__in_const0),
    .in0_valid(myconst__in_const0_valid),
    .out0(myconst__out1),
    .out0_valid(myconst__out1_valid)
  );

  wire [width-1:0] reg1__out1;
  wire reg1__out1_valid;

  reg_m
  #(
    .width(width)
  )
  reg1
  (
    .clk(clk),
    .rst(rst),
    .in0(add3__out1),
    .in0_valid(add3__out1_valid),
    .out0(reg1__out1),
    .out0_valid(reg1__out1_valid)
  );

  wire [width-1:0] reg2__out1;
  wire reg2__out1_valid;

  reg_m
  #(
    .width(width)
  )
  reg2
  (
    .clk(clk),
    .rst(rst),
    .in0(reg1__out1),
    .in0_valid(reg1__out1_valid),
    .out0(reg2__out1),
    .out0_valid(reg2__out1_valid)
  );


endmodule



module myadd #
(
  parameter width = 8
)
(
  input clk,
  input rst,
  input [width-1:0] in0,
  input in0_valid,
  input [width-1:0] in1,
  input in1_valid,
  input [width-1:0] in2,
  input in2_valid,
  output [width-1:0] reg1__out1,
  output reg1__out1_valid
);

  wire [width-1:0] add1__out1;
  wire add1__out1_valid;

  add_m
  #(
    .width(width)
  )
  add1
  (
    .clk(clk),
    .rst(rst),
    .in0(in0),
    .in0_valid(in0_valid),
    .in1(in1),
    .in1_valid(in1_valid),
    .out0(add1__out1),
    .out0_valid(add1__out1_valid)
  );

  wire [width-1:0] add2__out1;
  wire add2__out1_valid;

  add_m
  #(
    .width(width)
  )
  add2
  (
    .clk(clk),
    .rst(rst),
    .in0(in2),
    .in0_valid(in2_valid),
    .in1(add1__out1),
    .in1_valid(add1__out1_valid),
    .out0(add2__out1),
    .out0_valid(add2__out1_valid)
  );

  wire [width-1:0] reg1__out1;
  wire reg1__out1_valid;

  reg_m
  #(
    .width(width)
  )
  reg1
  (
    .clk(clk),
    .rst(rst),
    .in0(add2__out1),
    .in0_valid(add2__out1_valid),
    .out0(reg1__out1),
    .out0_valid(reg1__out1_valid)
  );


endmodule



module add_m #
(
  parameter width = 8
)
(
  input clk,
  input rst,
  input [width-1:0] in0,
  input in0_valid,
  input [width-1:0] in1,
  input in1_valid,
  output [width-1:0] out0,
  output out0_valid
);

  assign out0 = in0 + in1;
  assign out0_valid = in0_valid & in1_valid;

endmodule



module reg_m #
(
  parameter width = 8
)
(
  input clk,
  input rst,
  input [width-1:0] in0,
  input in0_valid,
  output [width-1:0] out0,
  output out0_valid
);

  reg [width-1:0] value;
  reg valid;

  always @(posedge clk) begin
    if(rst) begin
      valid <= 0;
    end else begin
      valid <= in0_valid;
    end
    value <= in0;
  end

  assign out0 = value;
  assign out0_valid = valid;

  initial begin
    value = 0;
    valid = 0;
  end


endmodule



module const_m #
(
  parameter width = 8
)
(
  input clk,
  input rst,
  input [width-1:0] in0,
  input in0_valid,
  output [width-1:0] out0,
  output out0_valid
);

  reg [width-1:0] value;
  reg valid;

  always @(posedge clk) begin
    if(rst) begin
      valid <= 0;
    end else begin
      if(in0_valid) begin
        valid <= 1'b1;
        value <= in0;
      end 
    end
  end

  assign out0 = value;
  assign out0_valid = valid;

  initial begin
    value = 0;
    valid = 0;
  end


endmodule



module sub_m #
(
  parameter width = 8
)
(
  input clk,
  input rst,
  input [width-1:0] in0,
  input in0_valid,
  input [width-1:0] in1,
  input in1_valid,
  output [width-1:0] out0,
  output out0_valid
);

  assign out0 = in0 - in1;
  assign out0_valid = in0_valid & in1_valid;

endmodule



module route_4_3x2 #
(
  parameter width = 16
)
(
  input [4-1:0] sel_in,
  input [width-1:0] in0,
  input [width-1:0] in1,
  input [width-1:0] in2,
  output [width-1:0] out0,
  output [width-1:0] out1
);


  switch_3_2
  #(
    .width(width)
  )
  switch_3_2
  (
    .sel(sel_in),
    .in0(in0),
    .in1(in1),
    .in2(in2),
    .out0(out0),
    .out1(out1)
  );


endmodule



module switch_3_2 #
(
  parameter width = 16
)
(
  input [4-1:0] sel,
  input [width-1:0] in0,
  input [width-1:0] in1,
  input [width-1:0] in2,
  output [width-1:0] out0,
  output [width-1:0] out1
);


  multiplexer_3
  #(
    .width(width)
  )
  mux0
  (
    .in0(in0),
    .in1(in1),
    .in2(in2),
    .sel(sel[1:0]),
    .out(out0)
  );


  multiplexer_3
  #(
    .width(width)
  )
  mux1
  (
    .in0(in0),
    .in1(in1),
    .in2(in2),
    .sel(sel[3:2]),
    .out(out1)
  );


endmodule



module multiplexer_3 #
(
  parameter width = 8
)
(
  input [2-1:0] sel,
  input [width-1:0] in0,
  input [width-1:0] in1,
  input [width-1:0] in2,
  output [width-1:0] out
);

  wire [width-1:0] aux [0:3-1];
  assign aux[0] = in0;
  assign aux[1] = in1;
  assign aux[2] = in2;
  assign out = aux[sel];

endmodule



module pe_conf_reader_alu_in_3_alu_w_11_router_w_4 #
(
  parameter pe_id = 0,
  parameter conf_raw_bits = 0
)
(
  input clk,
  input [9-1:0] conf_bus,
  output reg reset,
  output reg [11-1:0] conf_alu,
  output reg [51-1:0] conf_const,
  output reg [4-1:0] conf_router
);

  reg [9-1:0] conf_bus_r;
  reg conf_valid0;
  reg conf_valid1;
  reg conf_valid2;
  reg conf_valid;
  reg [22-1:0] conf_reg0;
  reg [22-1:0] conf_reg1;
  reg [22-1:0] conf_reg2;
  reg [22-1:0] conf_reg;
  reg [conf_raw_bits-1:0] conf_raw_reg;
  reg [conf_raw_bits/8-1:0] count;

  always @(posedge clk) begin
    conf_bus_r <= conf_bus;
  end


  always @(posedge clk) begin
    conf_valid0 <= 1'b0;
    conf_reg0 <= 22'b0;
    conf_raw_reg <= (conf_bus_r[0])? { conf_bus_r[8:1], conf_raw_reg[conf_raw_bits-1:8] } : { conf_raw_bits{ 1'b0 } };
    count <= (conf_bus_r[0])? { 1'b1, count[conf_raw_bits/8-1:1] } : { conf_raw_bits / 8{ 1'b0 } };
    if(count[0]) begin
      conf_reg0 <= conf_raw_reg[21:0];
      conf_valid0 <= 1'b1;
      count <= { conf_bus_r[0], { conf_raw_bits / 8 - 1{ 1'b0 } } };
    end 
  end


  always @(posedge clk) begin
    conf_reg1 <= conf_reg0;
    conf_reg2 <= conf_reg1;
    conf_reg <= conf_reg2;
    conf_valid1 <= conf_valid0;
    conf_valid2 <= conf_valid1;
    conf_valid <= conf_valid2;
  end


  always @(posedge clk) begin
    reset <= 1'b0;
    if(conf_valid && (pe_id == conf_reg[2:0])) begin
      case(conf_reg[5:3])
        3'b0: begin
          reset <= 1'b1;
          conf_alu <= 0;
          conf_const <= 0;
          conf_router <= 0;
        end
        3'b1: begin
          conf_alu <= conf_reg[16:6];
        end
        3'b10: begin
          conf_const[1*17-1:0*17] <= { 1'b1, conf_reg[21:6] };
        end
        3'b11: begin
          conf_const[2*17-1:1*17] <= { 1'b1, conf_reg[21:6] };
        end
        3'b100: begin
          conf_const[3*17-1:2*17] <= { 1'b1, conf_reg[21:6] };
        end
        3'b101: begin
          conf_router <= conf_reg[9:6];
        end
      endcase
    end 
  end


  initial begin
    reset = 0;
    conf_alu = 0;
    conf_const = 0;
    conf_router = 0;
    conf_bus_r = 0;
    conf_valid0 = 0;
    conf_valid1 = 0;
    conf_valid2 = 0;
    conf_valid = 0;
    conf_reg0 = 0;
    conf_reg1 = 0;
    conf_reg2 = 0;
    conf_reg = 0;
    conf_raw_reg = 0;
    count = 0;
  end


endmodule



module pei0o1n2r4e440aluadd_msub_m #
(
  parameter id = 0,
  parameter conf_raw_bits = 0
)
(
  input clk,
  input [9-1:0] conf_bus,
  input [17-1:0] in0,
  input [17-1:0] in1,
  output [17-1:0] out0,
  output [17-1:0] out1,
  output [17-1:0] stream_out0
);

  wire [17-1:0] in_reg0;
  wire [17-1:0] in_reg1;
  wire [17-1:0] router_out0;
  wire [17-1:0] router_out1;
  wire [17-1:0] stream_out_route0;
  wire reset;
  wire [2*17-1:0] pe_const;
  wire [17-1:0] in_reg_router0;
  wire [17-1:0] in_reg_router1;
  wire [17-1:0] mux_alu_out0;
  wire [17-1:0] mux_alu_out1;
  wire [17-1:0] alu_out0;
  reg [1-1:0] sel_alu_opcode;
  reg [2-1:0] sel_mux_alu0;
  reg [2-1:0] sel_mux_alu1;
  reg [6-1:0] route_sel_in;

  reg_pipe
  #(
    .num_register(1),
    .width(17)
  )
  in0_reg
  (
    .clk(clk),
    .rst(1'b0),
    .en(1'b1),
    .in(in0),
    .out(in_reg0)
  );


  reg_pipe
  #(
    .num_register(1),
    .width(17)
  )
  in1_reg
  (
    .clk(clk),
    .rst(1'b0),
    .en(1'b1),
    .in(in1),
    .out(in_reg1)
  );


  multiplexer_3
  #(
    .width(17)
  )
  mux_alu_in0
  (
    .sel(sel_mux_alu0),
    .in0(pe_const[1*17-1:0*17]),
    .in1(in_reg0),
    .in2(in_reg1),
    .out(mux_alu_out0)
  );

  wire [17-1:0] elastic_pipeline_to_alu0;
  reg [3-1:0] sel_elastic_pipeline0;

  elastic_pipeline_6_4
  #(
    .width(17)
  )
  elastic_pipeline0
  (
    .in(mux_alu_out0),
    .out(elastic_pipeline_to_alu0),
    .clk(clk),
    .en(1'b1),
    .latency(sel_elastic_pipeline0)
  );


  multiplexer_3
  #(
    .width(17)
  )
  mux_alu_in1
  (
    .sel(sel_mux_alu1),
    .in0(pe_const[2*17-1:1*17]),
    .in1(in_reg0),
    .in2(in_reg1),
    .out(mux_alu_out1)
  );

  wire [17-1:0] elastic_pipeline_to_alu1;
  reg [3-1:0] sel_elastic_pipeline1;

  elastic_pipeline_6_4
  #(
    .width(17)
  )
  elastic_pipeline1
  (
    .in(mux_alu_out1),
    .out(elastic_pipeline_to_alu1),
    .clk(clk),
    .en(1'b1),
    .latency(sel_elastic_pipeline1)
  );


  aluadd_msub_m
  #(
    .width(16)
  )
  alu
  (
    .clk(clk),
    .rst(reset),
    .opcode(sel_alu_opcode),
    .in0(elastic_pipeline_to_alu0),
    .in1(elastic_pipeline_to_alu1),
    .out0(alu_out0)
  );


  reg_pipe
  #(
    .num_register(4),
    .width(17)
  )
  in_reg0_router
  (
    .clk(clk),
    .rst(1'b0),
    .en(1'b1),
    .in(in_reg0),
    .out(in_reg_router0)
  );


  reg_pipe
  #(
    .num_register(4),
    .width(17)
  )
  in_reg1_router
  (
    .clk(clk),
    .rst(1'b0),
    .en(1'b1),
    .in(in_reg1),
    .out(in_reg_router1)
  );


  route_4_3x3
  #(
    .width(17)
  )
  router
  (
    .sel_in(route_sel_in),
    .in0(alu_out0),
    .in1(in_reg_router0),
    .in2(in_reg_router1),
    .out0(router_out0),
    .out1(router_out1),
    .out2(stream_out_route0)
  );

  wire [11-1:0] conf_alu;
  wire [6-1:0] conf_router;

  pe_conf_reader_alu_in_2_alu_w_11_router_w_6
  #(
    .pe_id(id),
    .conf_raw_bits(conf_raw_bits)
  )
  pe_conf_reader
  (
    .clk(clk),
    .conf_bus(conf_bus),
    .reset(reset),
    .conf_alu(conf_alu),
    .conf_const(pe_const),
    .conf_router(conf_router)
  );


  reg_pipe
  #(
    .num_register(1),
    .width(17)
  )
  out0_reg
  (
    .clk(clk),
    .rst(1'b0),
    .en(1'b1),
    .in(router_out0),
    .out(out0)
  );


  reg_pipe
  #(
    .num_register(1),
    .width(17)
  )
  out1_reg
  (
    .clk(clk),
    .rst(1'b0),
    .en(1'b1),
    .in(router_out1),
    .out(out1)
  );


  reg_pipe
  #(
    .num_register(1),
    .width(17)
  )
  stream_out0_reg
  (
    .clk(clk),
    .rst(1'b0),
    .en(1'b1),
    .in(stream_out_route0),
    .out(stream_out0)
  );


  always @(posedge clk) begin
    sel_alu_opcode <= conf_alu[0:0];
    sel_mux_alu0 <= conf_alu[2:1];
    sel_mux_alu1 <= conf_alu[4:3];
    sel_elastic_pipeline0 <= conf_alu[7:5];
    sel_elastic_pipeline1 <= conf_alu[10:8];
    route_sel_in <= conf_router[5:0];
  end


  initial begin
    sel_alu_opcode = 0;
    sel_mux_alu0 = 0;
    sel_mux_alu1 = 0;
    route_sel_in = 0;
    sel_elastic_pipeline0 = 0;
    sel_elastic_pipeline1 = 0;
  end


endmodule



module aluadd_msub_m #
(
  parameter width = 8
)
(
  input clk,
  input rst,
  input [1-1:0] opcode,
  input [width+1-1:0] in0,
  input [width+1-1:0] in1,
  output [width+1-1:0] out0
);

  reg [width+1-1:0] in_reg0;
  reg [width+1-1:0] in_reg1;
  wire [width+1-1:0] out_ops0 [0:2-1];
  wire [width+1-1:0] out_ops_reg0 [0:2-1];

  add_m
  #(
    .width(width)
  )
  add_m
  (
    .clk(clk),
    .rst(rst),
    .in0(in_reg0[width-1:0]),
    .in0_valid(in_reg0[width]),
    .in1(in_reg1[width-1:0]),
    .in1_valid(in_reg1[width]),
    .out0(out_ops0[0][width-1:0]),
    .out0_valid(out_ops0[0][width])
  );


  reg_pipe
  #(
    .num_register(3),
    .width(width + 1)
  )
  add_m_outreg
  (
    .clk(clk),
    .rst(1'b0),
    .en(1'b1),
    .in(out_ops0[0]),
    .out(out_ops_reg0[0])
  );


  sub_m
  #(
    .width(width)
  )
  sub_m
  (
    .clk(clk),
    .rst(rst),
    .in0(in_reg0[width-1:0]),
    .in0_valid(in_reg0[width]),
    .in1(in_reg1[width-1:0]),
    .in1_valid(in_reg1[width]),
    .out0(out_ops0[1][width-1:0]),
    .out0_valid(out_ops0[1][width])
  );


  reg_pipe
  #(
    .num_register(3),
    .width(width + 1)
  )
  sub_m_outreg
  (
    .clk(clk),
    .rst(1'b0),
    .en(1'b1),
    .in(out_ops0[1]),
    .out(out_ops_reg0[1])
  );


  always @(posedge clk) begin
    in_reg0 <= in0;
    in_reg1 <= in1;
  end

  assign out0 = out_ops_reg0[opcode];

  initial begin
    in_reg0 = 0;
    in_reg1 = 0;
  end


endmodule



module route_4_3x3 #
(
  parameter width = 16
)
(
  input [6-1:0] sel_in,
  input [width-1:0] in0,
  input [width-1:0] in1,
  input [width-1:0] in2,
  output [width-1:0] out0,
  output [width-1:0] out1,
  output [width-1:0] out2
);


  switch_3_3
  #(
    .width(width)
  )
  switch_3_3
  (
    .sel(sel_in),
    .in0(in0),
    .in1(in1),
    .in2(in2),
    .out0(out0),
    .out1(out1),
    .out2(out2)
  );


endmodule



module switch_3_3 #
(
  parameter width = 16
)
(
  input [6-1:0] sel,
  input [width-1:0] in0,
  input [width-1:0] in1,
  input [width-1:0] in2,
  output [width-1:0] out0,
  output [width-1:0] out1,
  output [width-1:0] out2
);


  multiplexer_3
  #(
    .width(width)
  )
  mux0
  (
    .in0(in0),
    .in1(in1),
    .in2(in2),
    .sel(sel[1:0]),
    .out(out0)
  );


  multiplexer_3
  #(
    .width(width)
  )
  mux1
  (
    .in0(in0),
    .in1(in1),
    .in2(in2),
    .sel(sel[3:2]),
    .out(out1)
  );


  multiplexer_3
  #(
    .width(width)
  )
  mux2
  (
    .in0(in0),
    .in1(in1),
    .in2(in2),
    .sel(sel[5:4]),
    .out(out2)
  );


endmodule



module pe_conf_reader_alu_in_2_alu_w_11_router_w_6 #
(
  parameter pe_id = 0,
  parameter conf_raw_bits = 0
)
(
  input clk,
  input [9-1:0] conf_bus,
  output reg reset,
  output reg [11-1:0] conf_alu,
  output reg [34-1:0] conf_const,
  output reg [6-1:0] conf_router
);

  reg [9-1:0] conf_bus_r;
  reg conf_valid0;
  reg conf_valid1;
  reg conf_valid2;
  reg conf_valid;
  reg [22-1:0] conf_reg0;
  reg [22-1:0] conf_reg1;
  reg [22-1:0] conf_reg2;
  reg [22-1:0] conf_reg;
  reg [conf_raw_bits-1:0] conf_raw_reg;
  reg [conf_raw_bits/8-1:0] count;

  always @(posedge clk) begin
    conf_bus_r <= conf_bus;
  end


  always @(posedge clk) begin
    conf_valid0 <= 1'b0;
    conf_reg0 <= 22'b0;
    conf_raw_reg <= (conf_bus_r[0])? { conf_bus_r[8:1], conf_raw_reg[conf_raw_bits-1:8] } : { conf_raw_bits{ 1'b0 } };
    count <= (conf_bus_r[0])? { 1'b1, count[conf_raw_bits/8-1:1] } : { conf_raw_bits / 8{ 1'b0 } };
    if(count[0]) begin
      conf_reg0 <= conf_raw_reg[21:0];
      conf_valid0 <= 1'b1;
      count <= { conf_bus_r[0], { conf_raw_bits / 8 - 1{ 1'b0 } } };
    end 
  end


  always @(posedge clk) begin
    conf_reg1 <= conf_reg0;
    conf_reg2 <= conf_reg1;
    conf_reg <= conf_reg2;
    conf_valid1 <= conf_valid0;
    conf_valid2 <= conf_valid1;
    conf_valid <= conf_valid2;
  end


  always @(posedge clk) begin
    reset <= 1'b0;
    if(conf_valid && (pe_id == conf_reg[2:0])) begin
      case(conf_reg[5:3])
        3'b0: begin
          reset <= 1'b1;
          conf_alu <= 0;
          conf_const <= 0;
          conf_router <= 0;
        end
        3'b1: begin
          conf_alu <= conf_reg[16:6];
        end
        3'b10: begin
          conf_const[1*17-1:0*17] <= { 1'b1, conf_reg[21:6] };
        end
        3'b11: begin
          conf_const[2*17-1:1*17] <= { 1'b1, conf_reg[21:6] };
        end
        3'b100: begin
          conf_router <= conf_reg[11:6];
        end
      endcase
    end 
  end


  initial begin
    reset = 0;
    conf_alu = 0;
    conf_const = 0;
    conf_router = 0;
    conf_bus_r = 0;
    conf_valid0 = 0;
    conf_valid1 = 0;
    conf_valid2 = 0;
    conf_valid = 0;
    conf_reg0 = 0;
    conf_reg1 = 0;
    conf_reg2 = 0;
    conf_reg = 0;
    conf_raw_reg = 0;
    count = 0;
  end


endmodule



module pei1o0n2r4e440aluadd_msub_m #
(
  parameter id = 0,
  parameter conf_raw_bits = 0
)
(
  input clk,
  input [9-1:0] conf_bus,
  input [17-1:0] in0,
  input [17-1:0] in1,
  output [17-1:0] out0,
  output [17-1:0] out1,
  input [17-1:0] stream_in0
);

  wire [17-1:0] in_reg0;
  wire [17-1:0] in_reg1;
  wire [17-1:0] router_out0;
  wire [17-1:0] router_out1;
  wire [17-1:0] stream_in_reg0;
  wire reset;
  wire [2*17-1:0] pe_const;
  wire [17-1:0] in_reg_router0;
  wire [17-1:0] in_reg_router1;
  wire [17-1:0] mux_alu_out0;
  wire [17-1:0] mux_alu_out1;
  wire [17-1:0] alu_out0;
  reg [1-1:0] sel_alu_opcode;
  reg [2-1:0] sel_mux_alu0;
  reg [2-1:0] sel_mux_alu1;
  reg [4-1:0] route_sel_in;

  reg_pipe
  #(
    .num_register(1),
    .width(17)
  )
  m_stream_in_reg0
  (
    .clk(clk),
    .rst(1'b0),
    .en(1'b1),
    .in(stream_in0),
    .out(stream_in_reg0)
  );


  reg_pipe
  #(
    .num_register(1),
    .width(17)
  )
  in0_reg
  (
    .clk(clk),
    .rst(1'b0),
    .en(1'b1),
    .in(in0),
    .out(in_reg0)
  );


  reg_pipe
  #(
    .num_register(1),
    .width(17)
  )
  in1_reg
  (
    .clk(clk),
    .rst(1'b0),
    .en(1'b1),
    .in(in1),
    .out(in_reg1)
  );


  multiplexer_4
  #(
    .width(17)
  )
  mux_alu_in0
  (
    .sel(sel_mux_alu0),
    .in0(stream_in_reg0),
    .in1(pe_const[1*17-1:0*17]),
    .in2(in_reg0),
    .in3(in_reg1),
    .out(mux_alu_out0)
  );

  wire [17-1:0] elastic_pipeline_to_alu0;
  reg [3-1:0] sel_elastic_pipeline0;

  elastic_pipeline_6_4
  #(
    .width(17)
  )
  elastic_pipeline0
  (
    .in(mux_alu_out0),
    .out(elastic_pipeline_to_alu0),
    .clk(clk),
    .en(1'b1),
    .latency(sel_elastic_pipeline0)
  );


  multiplexer_4
  #(
    .width(17)
  )
  mux_alu_in1
  (
    .sel(sel_mux_alu1),
    .in0(stream_in_reg0),
    .in1(pe_const[2*17-1:1*17]),
    .in2(in_reg0),
    .in3(in_reg1),
    .out(mux_alu_out1)
  );

  wire [17-1:0] elastic_pipeline_to_alu1;
  reg [3-1:0] sel_elastic_pipeline1;

  elastic_pipeline_6_4
  #(
    .width(17)
  )
  elastic_pipeline1
  (
    .in(mux_alu_out1),
    .out(elastic_pipeline_to_alu1),
    .clk(clk),
    .en(1'b1),
    .latency(sel_elastic_pipeline1)
  );


  aluadd_msub_m
  #(
    .width(16)
  )
  alu
  (
    .clk(clk),
    .rst(reset),
    .opcode(sel_alu_opcode),
    .in0(elastic_pipeline_to_alu0),
    .in1(elastic_pipeline_to_alu1),
    .out0(alu_out0)
  );


  reg_pipe
  #(
    .num_register(4),
    .width(17)
  )
  in_reg0_router
  (
    .clk(clk),
    .rst(1'b0),
    .en(1'b1),
    .in(in_reg0),
    .out(in_reg_router0)
  );


  reg_pipe
  #(
    .num_register(4),
    .width(17)
  )
  in_reg1_router
  (
    .clk(clk),
    .rst(1'b0),
    .en(1'b1),
    .in(in_reg1),
    .out(in_reg_router1)
  );


  route_4_3x2
  #(
    .width(17)
  )
  router
  (
    .sel_in(route_sel_in),
    .in0(alu_out0),
    .in1(in_reg_router0),
    .in2(in_reg_router1),
    .out0(router_out0),
    .out1(router_out1)
  );

  wire [11-1:0] conf_alu;
  wire [4-1:0] conf_router;

  pe_conf_reader_alu_in_2_alu_w_11_router_w_4
  #(
    .pe_id(id),
    .conf_raw_bits(conf_raw_bits)
  )
  pe_conf_reader
  (
    .clk(clk),
    .conf_bus(conf_bus),
    .reset(reset),
    .conf_alu(conf_alu),
    .conf_const(pe_const),
    .conf_router(conf_router)
  );


  reg_pipe
  #(
    .num_register(1),
    .width(17)
  )
  out0_reg
  (
    .clk(clk),
    .rst(1'b0),
    .en(1'b1),
    .in(router_out0),
    .out(out0)
  );


  reg_pipe
  #(
    .num_register(1),
    .width(17)
  )
  out1_reg
  (
    .clk(clk),
    .rst(1'b0),
    .en(1'b1),
    .in(router_out1),
    .out(out1)
  );


  always @(posedge clk) begin
    sel_alu_opcode <= conf_alu[0:0];
    sel_mux_alu0 <= conf_alu[2:1];
    sel_mux_alu1 <= conf_alu[4:3];
    sel_elastic_pipeline0 <= conf_alu[7:5];
    sel_elastic_pipeline1 <= conf_alu[10:8];
    route_sel_in <= conf_router[3:0];
  end


  initial begin
    sel_alu_opcode = 0;
    sel_mux_alu0 = 0;
    sel_mux_alu1 = 0;
    route_sel_in = 0;
    sel_elastic_pipeline0 = 0;
    sel_elastic_pipeline1 = 0;
  end


endmodule



module pe_conf_reader_alu_in_2_alu_w_11_router_w_4 #
(
  parameter pe_id = 0,
  parameter conf_raw_bits = 0
)
(
  input clk,
  input [9-1:0] conf_bus,
  output reg reset,
  output reg [11-1:0] conf_alu,
  output reg [34-1:0] conf_const,
  output reg [4-1:0] conf_router
);

  reg [9-1:0] conf_bus_r;
  reg conf_valid0;
  reg conf_valid1;
  reg conf_valid2;
  reg conf_valid;
  reg [22-1:0] conf_reg0;
  reg [22-1:0] conf_reg1;
  reg [22-1:0] conf_reg2;
  reg [22-1:0] conf_reg;
  reg [conf_raw_bits-1:0] conf_raw_reg;
  reg [conf_raw_bits/8-1:0] count;

  always @(posedge clk) begin
    conf_bus_r <= conf_bus;
  end


  always @(posedge clk) begin
    conf_valid0 <= 1'b0;
    conf_reg0 <= 22'b0;
    conf_raw_reg <= (conf_bus_r[0])? { conf_bus_r[8:1], conf_raw_reg[conf_raw_bits-1:8] } : { conf_raw_bits{ 1'b0 } };
    count <= (conf_bus_r[0])? { 1'b1, count[conf_raw_bits/8-1:1] } : { conf_raw_bits / 8{ 1'b0 } };
    if(count[0]) begin
      conf_reg0 <= conf_raw_reg[21:0];
      conf_valid0 <= 1'b1;
      count <= { conf_bus_r[0], { conf_raw_bits / 8 - 1{ 1'b0 } } };
    end 
  end


  always @(posedge clk) begin
    conf_reg1 <= conf_reg0;
    conf_reg2 <= conf_reg1;
    conf_reg <= conf_reg2;
    conf_valid1 <= conf_valid0;
    conf_valid2 <= conf_valid1;
    conf_valid <= conf_valid2;
  end


  always @(posedge clk) begin
    reset <= 1'b0;
    if(conf_valid && (pe_id == conf_reg[2:0])) begin
      case(conf_reg[5:3])
        3'b0: begin
          reset <= 1'b1;
          conf_alu <= 0;
          conf_const <= 0;
          conf_router <= 0;
        end
        3'b1: begin
          conf_alu <= conf_reg[16:6];
        end
        3'b10: begin
          conf_const[1*17-1:0*17] <= { 1'b1, conf_reg[21:6] };
        end
        3'b11: begin
          conf_const[2*17-1:1*17] <= { 1'b1, conf_reg[21:6] };
        end
        3'b100: begin
          conf_router <= conf_reg[9:6];
        end
      endcase
    end 
  end


  initial begin
    reset = 0;
    conf_alu = 0;
    conf_const = 0;
    conf_router = 0;
    conf_bus_r = 0;
    conf_valid0 = 0;
    conf_valid1 = 0;
    conf_valid2 = 0;
    conf_valid = 0;
    conf_reg0 = 0;
    conf_reg1 = 0;
    conf_reg2 = 0;
    conf_reg = 0;
    conf_raw_reg = 0;
    count = 0;
  end


endmodule

