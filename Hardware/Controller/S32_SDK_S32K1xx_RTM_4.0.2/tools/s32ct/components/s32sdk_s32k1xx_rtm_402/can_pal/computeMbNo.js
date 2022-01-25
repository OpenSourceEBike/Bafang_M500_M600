function computeMbNo () {
    var configSet = child.getChildContext().getConfigSetConfig(); 
    var rxFifoFiltersElement = configSet.getChildById("can_fifo_ext2.struct_extension.numIdFilters");
	var rxFifoFilters = rxFifoFiltersElement.getValue();
    
    var ArrayList = Java.type('java.util.ArrayList');
	var rxFifoFiltersArray = new ArrayList();
    rxFifoFiltersArray.add("FLEXCAN_RX_FIFO_ID_FILTERS_8");
    rxFifoFiltersArray.add("FLEXCAN_RX_FIFO_ID_FILTERS_16");
    rxFifoFiltersArray.add("FLEXCAN_RX_FIFO_ID_FILTERS_24");
    rxFifoFiltersArray.add("FLEXCAN_RX_FIFO_ID_FILTERS_32");
    rxFifoFiltersArray.add("FLEXCAN_RX_FIFO_ID_FILTERS_40");
    rxFifoFiltersArray.add("FLEXCAN_RX_FIFO_ID_FILTERS_48");
    rxFifoFiltersArray.add("FLEXCAN_RX_FIFO_ID_FILTERS_56");
    rxFifoFiltersArray.add("FLEXCAN_RX_FIFO_ID_FILTERS_64");
    rxFifoFiltersArray.add("FLEXCAN_RX_FIFO_ID_FILTERS_72");
    rxFifoFiltersArray.add("FLEXCAN_RX_FIFO_ID_FILTERS_80");
    rxFifoFiltersArray.add("FLEXCAN_RX_FIFO_ID_FILTERS_88");
    rxFifoFiltersArray.add("FLEXCAN_RX_FIFO_ID_FILTERS_96");
    rxFifoFiltersArray.add("FLEXCAN_RX_FIFO_ID_FILTERS_104");
    rxFifoFiltersArray.add("FLEXCAN_RX_FIFO_ID_FILTERS_112");
    rxFifoFiltersArray.add("FLEXCAN_RX_FIFO_ID_FILTERS_120");
    rxFifoFiltersArray.add("FLEXCAN_RX_FIFO_ID_FILTERS_128");
    
    var idx = rxFifoFiltersArray.indexOf(rxFifoFilters);
    
    var mbNoArray = new ArrayList();
    mbNoArray.add(8);    mbNoArray.add(10);    mbNoArray.add(12);    mbNoArray.add(14);    mbNoArray.add(16);    mbNoArray.add(18);    mbNoArray.add(20);    mbNoArray.add(22);    mbNoArray.add(24);    mbNoArray.add(26);    mbNoArray.add(28);    mbNoArray.add(30);    mbNoArray.add(32);    mbNoArray.add(34);    mbNoArray.add(36);    mbNoArray.add(38);
    
    return mbNoArray[idx];
}

computeMbNo()