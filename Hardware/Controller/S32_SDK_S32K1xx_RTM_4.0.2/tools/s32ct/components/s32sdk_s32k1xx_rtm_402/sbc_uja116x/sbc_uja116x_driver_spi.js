/*
 * Copyright 2019-2020 NXP
 * To be used with S32 Configuration Tool. See Software License Agreement of S32 Configuration Tool.
 */

function runValidation(defaultLpspiCfg) {
  var components = child.getChildContext().getFunctionalGroup().getChildren();
  var spiComp;
  var bitsPerSec, pcsPolarity, isPcsContinuous, bitcount, clkPhase, clkPolarity, lsbFirst;

  for (var i = 0; i < components.length; i++)
  {
    if (components.get(i).getType() == "lpspi_config")
    {
      spiComp = components.get(i);
      if (spiComp.getName() == child.getChildById("sbcSpi").getChildById("spi_t").getValue())
      {
        bitsPerSec = spiComp.getChildById("lpspi").getChildById("lpspiMasterConfig").getChildren().get(0).getChildById("bitsPerSec");
        pcsPolarity = spiComp.getChildById("lpspi").getChildById("lpspiMasterConfig").getChildren().get(0).getChildById("pcsPolarity");
        isPcsContinuous = spiComp.getChildById("lpspi").getChildById("lpspiMasterConfig").getChildren().get(0).getChildById("isPcsContinuous");
        bitcount = spiComp.getChildById("lpspi").getChildById("lpspiMasterConfig").getChildren().get(0).getChildById("bitcount");
        clkPhase = spiComp.getChildById("lpspi").getChildById("lpspiMasterConfig").getChildren().get(0).getChildById("clkPhase");
        clkPolarity = spiComp.getChildById("lpspi").getChildById("lpspiMasterConfig").getChildren().get(0).getChildById("clkPolarity");
        lsbFirst = spiComp.getChildById("lpspi").getChildById("lpspiMasterConfig").getChildren().get(0).getChildById("lsbFirst");

        if (defaultLpspiCfg.getValue() == 1)
        {
          pcsPolarity.setValue("LPSPI_ACTIVE_LOW");
          isPcsContinuous.setValue("false");
          bitcount.setValue(16);
          clkPhase.setValue("LPSPI_CLOCK_PHASE_2ND_EDGE");
          clkPolarity.setValue("LPSPI_SCK_ACTIVE_HIGH");
          lsbFirst.setValue("false");
          defaultLpspiCfg.setValue(0);
        }
      }
    }
  }

  return [bitsPerSec, pcsPolarity, isPcsContinuous, bitcount, clkPhase, clkPolarity, lsbFirst];
}