function ewmPeriod() {
	
	return Math.floor(((child.getChildById("compareHigh").getValue() * (child.getChildById("prescaler").getValue() + 1)) / child.getChildById("LPOClkFreq").getValue()) * 1000).toString();
}

function ewmWindow() {
	
	return Math.floor((((child.getChildById("compareHigh").getValue() - child.getChildById("compareLow").getValue()) * (child.getChildById("prescaler").getValue() + 1)) / child.getChildById("LPOClkFreq").getValue()) * 1000).toString();
}