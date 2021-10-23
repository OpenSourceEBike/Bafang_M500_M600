
function instGetter() {
    var ArrayList = Java.type('java.util.ArrayList');
    PLATFORM = {};
    PLATFORM['S32K142W'] = {
        0:{
            has_fd : true,
            has_pe_clk_select : true,
            has_enhanced_fifo : false,
            has_pn : true,
            clock_type : 'FLEXCAN{0}_CLK',
            module_clock : 'SYS_CLK',
            pe_clock : new ArrayList(['SOSCDIV2_CLK', 'SYS_CLK']),
            mbs : 64,
            embs : 0,
            dma_support : true,
            dma_range : new ArrayList([0, 15])
        },
        1:{
            has_fd : false,
            has_pe_clk_select : true,
            has_enhanced_fifo : false,
            has_pn : false,
            clock_type : 'FLEXCAN{0}_CLK',
            module_clock : 'SYS_CLK',
            pe_clock : new ArrayList(['SOSCDIV2_CLK', 'SYS_CLK']),
            mbs : 64,
            embs : 0,
            dma_support : true,
            dma_range : new ArrayList([0, 15])
        }
    };
    
    PLATFORM['S32K144W'] = {
        0:{
            has_fd : true,
            has_pe_clk_select : true,
            has_enhanced_fifo : false,
            has_pn : true,
            clock_type : 'FLEXCAN{0}_CLK',
            module_clock : 'SYS_CLK',
            pe_clock : new ArrayList(['SOSCDIV2_CLK', 'SYS_CLK']),
            mbs : 64,
            embs : 0,
            dma_support : true,
            dma_range : new ArrayList([0, 15])
        },
        1:{
            has_fd : true,
            has_pe_clk_select : true,
            has_enhanced_fifo : false,
            has_pn : false,
            clock_type : 'FLEXCAN{0}_CLK',
            module_clock : 'SYS_CLK',
            pe_clock : new ArrayList(['SOSCDIV2_CLK', 'SYS_CLK']),
            mbs : 64,
            embs : 0,
            dma_support : true,
            dma_range : new ArrayList([0, 15])
        }
    };
    
    PLATFORM['S32K142'] = {
        0:{
            has_fd : true,
            has_pe_clk_select : true,
            has_enhanced_fifo : false,
            has_pn : true,
            clock_type : 'FLEXCAN{0}_CLK',
            module_clock : 'SYS_CLK',
            pe_clock : new ArrayList(['SOSCDIV2_CLK', 'SYS_CLK']),
            mbs : 32,
            embs : 0,
            dma_support : true,
            dma_range : new ArrayList([0, 15])
        },
        1:{
            has_fd : false,
            has_pe_clk_select : true,
            has_enhanced_fifo : false,
            has_pn : false,
            clock_type : 'FLEXCAN{0}_CLK',
            module_clock : 'SYS_CLK',
            pe_clock : new ArrayList(['SOSCDIV2_CLK', 'SYS_CLK']),
            mbs :16,
            embs : 0,
            dma_support : true,
            dma_range : new ArrayList([0, 15])
        }
    };
    
    PLATFORM['S32K144'] = {
        0:{
            has_fd : true,
            has_pe_clk_select : true,
            has_enhanced_fifo : false,
            has_pn : true,
            clock_type : 'FLEXCAN{0}_CLK',
            module_clock : 'SYS_CLK',
            pe_clock : new ArrayList(['SOSCDIV2_CLK', 'SYS_CLK']),
            mbs : 32,
            embs : 0,
            dma_support : true,
            dma_range : new ArrayList([0, 15])
        },
        1:{
            has_fd : false,
            has_pe_clk_select : true,
            has_enhanced_fifo : false,
            has_pn : false,
            clock_type : 'FLEXCAN{0}_CLK',
            module_clock : 'SYS_CLK',
            pe_clock : new ArrayList(['SOSCDIV2_CLK', 'SYS_CLK']),
            mbs : 16,
            embs : 0,
            dma_support : true,
            dma_range : new ArrayList([0, 15])
        },
        2:{
            has_fd : false,
            has_pe_clk_select : true,
            has_enhanced_fifo : false,
            has_pn : false,
            clock_type : 'FLEXCAN{0}_CLK',
            module_clock : 'SYS_CLK',
            pe_clock : new ArrayList(['SOSCDIV2_CLK', 'SYS_CLK']),
            mbs : 16,
            embs : 0,
            dma_support : true,
            dma_range : new ArrayList([0, 15])
        }
    };
    
    PLATFORM['S32K146'] = {
        0:{
            has_fd : true,
            has_pe_clk_select : true,
            has_enhanced_fifo : false,
            has_pn : true,
            clock_type : 'FLEXCAN{0}_CLK',
            module_clock : 'SYS_CLK',
            pe_clock : new ArrayList(['SOSCDIV2_CLK', 'SYS_CLK']),
            mbs : 32,
            embs : 0,
            dma_support : true,
            dma_range : new ArrayList([0, 15])
        },
        1:{
            has_fd : true,
            has_pe_clk_select : true,
            has_enhanced_fifo : false,
            has_pn : false,
            clock_type : 'FLEXCAN{0}_CLK',
            module_clock : 'SYS_CLK',
            pe_clock : new ArrayList(['SOSCDIV2_CLK', 'SYS_CLK']),
            mbs : 32,
            embs : 0,
            dma_support : true,
            dma_range : new ArrayList([0, 15])
        },
        2:{
            has_fd : false,
            has_pe_clk_select : true,
            has_enhanced_fifo : false,
            has_pn : false,
            clock_type : 'FLEXCAN{0}_CLK',
            module_clock : 'SYS_CLK',
            pe_clock : new ArrayList(['SOSCDIV2_CLK', 'SYS_CLK']),
            mbs : 16,
            embs : 0,
            dma_support : true,
            dma_range : new ArrayList([0, 15])
        }
    };
    
    PLATFORM['S32K148'] = {
        0:{
            has_fd : true,
            has_pe_clk_select : true,
            has_enhanced_fifo : false,
            has_pn : true,
            clock_type : 'FLEXCAN{0}_CLK',
            module_clock : 'SYS_CLK',
            pe_clock : new ArrayList(['SOSCDIV2_CLK', 'SYS_CLK']),
            mbs : 32,
            embs : 0,
            dma_support : true,
            dma_range : new ArrayList([0, 15])
        },
        1:{
            has_fd : true,
            has_pe_clk_select : true,
            has_enhanced_fifo : false,
            has_pn : false,
            clock_type : 'FLEXCAN{0}_CLK',
            module_clock : 'SYS_CLK',
            pe_clock : new ArrayList(['SOSCDIV2_CLK', 'SYS_CLK']),
            mbs : 32,
            embs : 0,
            dma_support : true,
            dma_range : new ArrayList([0, 15])
        },
        2:{
            has_fd : true,
            has_pe_clk_select : true,
            has_enhanced_fifo : false,
            has_pn : false,
            clock_type : 'FLEXCAN{0}_CLK',
            module_clock : 'SYS_CLK',
            pe_clock : new ArrayList(['SOSCDIV2_CLK', 'SYS_CLK']),
            mbs : 32,
            embs : 0,
            dma_support : true,
            dma_range : new ArrayList([0, 15])
        }
    };
        
    PLATFORM['S32K116'] = {
        0:{
            has_fd : true,
            has_pe_clk_select : true,
            has_enhanced_fifo : false,
            has_pn : true,
            clock_type : 'FLEXCAN{0}_CLK',
            module_clock : 'SYS_CLK',
            pe_clock : new ArrayList(['SOSCDIV2_CLK', 'SYS_CLK']),
            mbs : 32,
            embs : 0,
            dma_support : true,
            dma_range : new ArrayList([0, 15])
        }
    };
    
    PLATFORM['S32K118'] = {
        0:{
            has_fd : true,
            has_pe_clk_select : true,
            has_enhanced_fifo : false,
            has_pn : true,
            clock_type : 'FLEXCAN{0}_CLK',
            module_clock : 'SYS_CLK',
            pe_clock : new ArrayList(['SOSCDIV2_CLK', 'SYS_CLK']),
            mbs : 32,
            embs : 0,
            dma_support : true,
            dma_range : new ArrayList([0, 15])
        }
    };
    
    PLATFORM['S32V234'] = {
        0:{
            has_fd : true,
            has_pe_clk_select : false,
            has_enhanced_fifo : false,
            has_pn : false,
            clock_type : 'FLEXCAN{0}_CLK',
            module_clock : 'SYS6_CLK',
            pe_clock : new ArrayList(['FLEXCAN{0}_CLK']),
            mbs : 64,
            embs : 0,
            dma_support : true,
            dma_range : new ArrayList([0, 15])
        },
        1:{
            has_fd : true,
            has_pe_clk_select : false,
            has_enhanced_fifo : false,
            has_pn : false,
            clock_type : 'FLEXCAN{0}_CLK',
            module_clock : 'SYS6_CLK',
            pe_clock : new ArrayList(['FLEXCAN{0}_CLK']),
            mbs : 64,
            embs : 0,
            dma_support : true,
            dma_range : new ArrayList([0, 15])
        }
    };
    
    PLATFORM['S32G274A'] = {
        0:{
            has_fd : true,
            has_pe_clk_select : false,
            has_enhanced_fifo : true,
            has_pn : false,
            clock_type : 'CAN_{0}',
            module_clock : 'XBAR_DIV3_CLK',
            pe_clock : new ArrayList(['FLEXCAN{0}_CLK']),
            mbs : 128,
            embs : 0,
            dma_support : true,
            dma_range : new ArrayList([0, 15])
        },
        1:{
            has_fd : true,
            has_pe_clk_select : false,
            has_enhanced_fifo : true,
            has_pn : false,
            clock_type : 'CAN_{0}',
            module_clock : 'XBAR_DIV3_CLK',
            pe_clock : new ArrayList(['FLEXCAN{0}_CLK']),
            mbs : 128,
            embs : 0,
            dma_support : true,
            dma_range : new ArrayList([0, 15])
        },
        2:{
            has_fd : true,
            has_pe_clk_select : false,
            has_enhanced_fifo : true,
            has_pn : false,
            clock_type : 'CAN_{0}',
            module_clock : 'XBAR_DIV3_CLK',
            pe_clock : new ArrayList(['FLEXCAN{0}_CLK']),
            mbs : 128,
            embs : 0,
            dma_support : true,
            dma_range : new ArrayList([16, 31])
        },
        3:{
            has_fd : true,
            has_pe_clk_select : false,
            has_enhanced_fifo : true,
            has_pn : false,
            clock_type : 'CAN_{0}',
            module_clock : 'XBAR_DIV3_CLK',
            pe_clock : new ArrayList(['FLEXCAN{0}_CLK']),
            mbs : 128,
            embs : 0,
            dma_support : true,
            dma_range : new ArrayList([16, 31])
        },
        4:{
            has_fd : true,
            has_pe_clk_select : false,
            has_enhanced_fifo : true,
            has_pn : false,
            clock_type : 'CAN_{0}',
            module_clock : 'XBAR_DIV3_CLK',
            pe_clock : new ArrayList(['FLEXCAN{0}_CLK']),
            mbs : 128,
            embs : 0,
            dma_support : true,
            dma_range : new ArrayList([0, 15])
        },
        5:{
            has_fd : true,
            has_pe_clk_select : false,
            has_enhanced_fifo : true,
            has_pn : false,
            clock_type : 'CAN_{0}',
            module_clock : 'XBAR_DIV3_CLK',
            pe_clock : new ArrayList(['FLEXCAN{0}_CLK']),
            mbs : 128,
            embs : 0,
            dma_support : true,
            dma_range : new ArrayList([0, 15])
        },
        6:{
            has_fd : true,
            has_pe_clk_select : false,
            has_enhanced_fifo : true,
            has_pn : false,
            clock_type : 'CAN_{0}',
            module_clock : 'XBAR_DIV3_CLK',
            pe_clock : new ArrayList(['FLEXCAN{0}_CLK']),
            mbs : 128,
            embs : 0,
            dma_support : true,
            dma_range : new ArrayList([16, 31])
        },
        7:{
            has_fd : true,
            has_pe_clk_select : false,
            has_enhanced_fifo : true,
            has_pn : false,
            clock_type : 'CAN_{0}',
            module_clock : 'XBAR_DIV3_CLK',
            pe_clock : new ArrayList(['FLEXCAN{0}_CLK']),
            mbs : 128,
            embs : 0,
            dma_support : true,
            dma_range : new ArrayList([16, 31])
        },
    };
    
    PLATFORM['S32S247TV'] = {
        0:{
            has_fd : true,
            has_pe_clk_select : false,
            has_enhanced_fifo : true,
            has_pn : false,
            clock_type : 'CAN_{0}',
            module_clock : 'XBAR_DIV3_CLK',
            pe_clock : new ArrayList(['FLEXCAN{0}_CLK']),
            mbs : 128,
            embs : 128,
            dma_support : true,
            dma_range : new ArrayList([0, 15])
        },
        1:{
            has_fd : true,
            has_pe_clk_select : false,
            has_enhanced_fifo : true,
            has_pn : false,
            clock_type : 'CAN_{0}',
            module_clock : 'XBAR_DIV3_CLK',
            pe_clock : new ArrayList(['FLEXCAN{0}_CLK']),
            mbs : 128,
            embs : 128,
            dma_support : true,
            dma_range : new ArrayList([0, 15])
        },
        2:{
            has_fd : true,
            has_pe_clk_select : false,
            has_enhanced_fifo : true,
            has_pn : false,
            clock_type : 'CAN_{0}',
            module_clock : 'XBAR_DIV3_CLK',
            pe_clock : new ArrayList(['FLEXCAN{0}_CLK']),
            mbs : 128,
            embs : 128,
            dma_support : true,
            dma_range : new ArrayList([16, 31])
        },
        3:{
            has_fd : true,
            has_pe_clk_select : false,
            has_enhanced_fifo : true,
            has_pn : false,
            clock_type : 'CAN_{0}',
            module_clock : 'XBAR_DIV3_CLK',
            pe_clock : new ArrayList(['FLEXCAN{0}_CLK']),
            mbs : 128,
            embs : 128,
            dma_support : true,
            dma_range : new ArrayList([16, 31])
        },
        4:{
            has_fd : true,
            has_pe_clk_select : false,
            has_enhanced_fifo : false,
            has_pn : false,
            clock_type : 'CAN_{0}',
            module_clock : 'XBAR_DIV3_CLK',
            pe_clock : new ArrayList(['FLEXCAN{0}_CLK']),
            mbs : 128,
            embs : 0,
            dma_support : true,
            dma_range : new ArrayList([0, 15])
        },
        5:{
            has_fd : true,
            has_pe_clk_select : false,
            has_enhanced_fifo : false,
            has_pn : false,
            clock_type : 'CAN_{0}',
            module_clock : 'XBAR_DIV3_CLK',
            pe_clock : new ArrayList(['FLEXCAN{0}_CLK']),
            mbs : 128,
            embs : 0,
            dma_support : true,
            dma_range : new ArrayList([0, 15])
        },
        6:{
            has_fd : true,
            has_pe_clk_select : false,
            has_enhanced_fifo : false,
            has_pn : false,
            clock_type : 'CAN_{0}',
            module_clock : 'XBAR_DIV3_CLK',
            pe_clock : new ArrayList(['FLEXCAN{0}_CLK']),
            mbs : 128,
            embs : 0,
            dma_support : true,
            dma_range : new ArrayList([16, 31])
        },
        7:{
            has_fd : true,
            has_pe_clk_select : false,
            has_enhanced_fifo : false,
            has_pn : false,
            clock_type : 'CAN_{0}',
            module_clock : 'XBAR_DIV3_CLK',
            pe_clock : new ArrayList(['FLEXCAN{0}_CLK']),
            mbs : 128,
            embs : 0,
            dma_support : true,
            dma_range : new ArrayList([16, 31])
        },
    };
    
    PLATFORM['S32R45'] = {
        0:{
            has_fd : true,
            has_pe_clk_select : false,
            has_enhanced_fifo : true,
            has_pn : false,
            clock_type : 'CAN_{0}',
            module_clock : 'XBAR_DIV3_CLK',
            pe_clock : new ArrayList(['FLEXCAN{0}_CLK']),
            mbs : 128,
            embs : 128,
            dma_support : true,
            dma_range : new ArrayList([0, 15])
        },
        1:{
            has_fd : true,
            has_pe_clk_select : false,
            has_enhanced_fifo : true,
            has_pn : false,
            clock_type : 'CAN_{0}',
            module_clock : 'XBAR_DIV3_CLK',
            pe_clock : new ArrayList(['FLEXCAN{0}_CLK']),
            mbs : 128,
            embs : 128,
            dma_support : true,
            dma_range : new ArrayList([0, 15])
        },
        2:{
            has_fd : true,
            has_pe_clk_select : false,
            has_enhanced_fifo : true,
            has_pn : false,
            clock_type : 'CAN_{0}',
            module_clock : 'XBAR_DIV3_CLK',
            pe_clock : new ArrayList(['FLEXCAN{0}_CLK']),
            mbs : 128,
            embs : 128,
            dma_support : true,
            dma_range : new ArrayList([16, 31])
        },
        3:{
            has_fd : true,
            has_pe_clk_select : false,
            has_enhanced_fifo : true,
            has_pn : false,
            clock_type : 'CAN_{0}',
            module_clock : 'XBAR_DIV3_CLK',
            pe_clock : new ArrayList(['FLEXCAN{0}_CLK']),
            mbs : 128,
            embs : 128,
            dma_support : true,
            dma_range : new ArrayList([16, 31])
        },
        4:{
            has_fd : true,
            has_pe_clk_select : false,
            has_enhanced_fifo : true,
            has_pn : false,
            clock_type : 'CAN_{0}',
            module_clock : 'XBAR_DIV3_CLK',
            pe_clock : new ArrayList(['FLEXCAN{0}_CLK']),
            mbs : 128,
            embs : 128,
            dma_support : true,
            dma_range : new ArrayList([0, 15])
        },
        5:{
            has_fd : true,
            has_pe_clk_select : false,
            has_enhanced_fifo : true,
            has_pn : false,
            clock_type : 'CAN_{0}',
            module_clock : 'XBAR_DIV3_CLK',
            pe_clock : new ArrayList(['FLEXCAN{0}_CLK']),
            mbs : 128,
            embs : 128,
            dma_support : true,
            dma_range : new ArrayList([0, 15])
        },
        6:{
            has_fd : true,
            has_pe_clk_select : false,
            has_enhanced_fifo : true,
            has_pn : false,
            clock_type : 'CAN_{0}',
            module_clock : 'XBAR_DIV3_CLK',
            pe_clock : new ArrayList(['FLEXCAN{0}_CLK']),
            mbs : 128,
            embs : 128,
            dma_support : true,
            dma_range : new ArrayList([16, 31])
        },
        7:{
            has_fd : true,
            has_pe_clk_select : false,
            has_enhanced_fifo : true,
            has_pn : false,
            clock_type : 'CAN_{0}',
            module_clock : 'XBAR_DIV3_CLK',
            pe_clock : new ArrayList(['FLEXCAN{0}_CLK']),
            mbs : 128,
            embs : 128,
            dma_support : true,
            dma_range : new ArrayList([16, 31])
        },
    };
    
    var configSet = child.getChildContext().getConfigSetConfig();
	var cpuName = configSet.getChildById("flexcanCfg.cpu_name");
	var instNum = configSet.getChildById("flexcanCfg.inst_num");
    
    return PLATFORM[cpuName.getValue()][instNum.getValue()];
}

instGetter();
