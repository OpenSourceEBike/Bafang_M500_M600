   function calcBautRate()
   {
    var prediv_max, min_tq, max_tq, t_seg1_min, t_seg1_max;
    var t_seg2_min, t_seg2_max, propseg_max, pseg1_max, pseg2_max;
    var real_bitrate, real_sample;
    var found_values = false;

    var configSet = child.getChildContext().getConfigSetConfig();
    var fd = configSet.getChildById("can_user_config_t2.enableFD");
    var fd_value = fd.getValue();
     var bit2Seg = configSet.getChildById("can_user_config_t2.can_bitrate2ts");
    var bit2Seg_value = bit2Seg.getValue();

    /* Get PE clock when CAN PAL over FLEXCAN */
    if (configSet.getChildById("can_user_config_t2.peClkSrc").isAvailable())
    {
        if(configSet.getChildById("can_user_config_t2.peClkSrc").getValue() == 'CAN_CLK_SOURCE_OSC')
            var peClk = configSet.getChildById("can_user_config_t2.pe_clock_frequency");
        else
            var peClk = configSet.getChildById("can_user_config_t2.pe_clock_frequency01");
    }
    else
    {
        if (configSet.getChildById("can_user_config_t2.pe_clock_frequency_0").isAvailable())
            var peClk = configSet.getChildById("can_user_config_t2.pe_clock_frequency_0");
        else
            var peClk = configSet.getChildById("can_user_config_t2.pe_clock_frequency_1");
    }

    var peClk_value = peClk.getValue();
    if (bit2Seg_value = true)
    {
        var bitrate = configSet.getChildById("can_user_config_t2.flexcan_cfg_time_segments2.nominalBitrate.bitrate");
        var bitrate_value = bitrate.getValue();
        var sample_point = configSet.getChildById("can_user_config_t2.flexcan_cfg_time_segments2.nominalBitrate.samplingPoint");
        var sample_point_value = sample_point.getValue();
    }
    else
    {
        var bitrate = configSet.getChildById("can_user_config_t2.flexcan_cfg_time_segments1.nominalBitrate.bitrate");
        var bitrate_value = bitrate.getValue();
        var sample_point = configSet.getChildById("can_user_config_t2.flexcan_cfg_time_segments1.nominalBitrate.samplingPoint");
        var sample_point_value = sample_point.getValue();
    }

    if(fd_value = true)
    {
        nr_of_itereations = 2;
    }
    else
    {
        nr_of_itereations = 1;
    }


    var pseg1, pseg2, propseg, presdiv;


    var ArrayList = Java.type('java.util.ArrayList');
    var result = new ArrayList();
    for (var iteration=0; iteration < nr_of_itereations; iteration++)
    {
            /* Save the values that are closest to the given bitrate and sampling point */
        var min_dsample = Number.MAX_VALUE;
        var min_dbitrate = Number.MAX_VALUE;
        found_values = false;
        var fd_enable = false;
        if (iteration == 1)
        {
            prediv_max = 1024;
            min_tq = 5;
            max_tq = 48;
            t_seg1_min = 2;
            t_seg1_max = 39;
            t_seg2_min = 2;
            t_seg2_max = 8;
            propseg_max = 31;
            pseg1_max = 7;
            pseg2_max = 7;
            bitrate = configSet.getChildById("can_user_config_t2.flexcan_cfg_time_segments2.dataBitrate_.bitrate");
            bitrate_value = bitrate.getValue();
            sample_point = configSet.getChildById("can_user_config_t2.flexcan_cfg_time_segments2.dataBitrate_.samplingPoint");
            sample_point_value = sample_point.getValue();
            fd_enable = true;
        }
        else
        {
            prediv_max = 256;
            min_tq = 8;
            max_tq = 25;
            t_seg1_min = 4;
            t_seg1_max = 16;
            t_seg2_min = 2;
            t_seg2_max = 8;
            propseg_max = 7;
            pseg1_max = 7;
            pseg2_max = 7;
            bitrate = configSet.getChildById("can_user_config_t2.flexcan_cfg_time_segments2.nominalBitrate.bitrate");
            bitrate_value = bitrate.getValue();
            sample_point = configSet.getChildById("can_user_config_t2.flexcan_cfg_time_segments2.nominalBitrate.samplingPoint");
            sample_point_value = sample_point.getValue();
        }
        for (var tmp_presdiv = 0; tmp_presdiv < prediv_max; tmp_presdiv++)
        {
            var tmp_pseg1, tmp_pseg2, tmp_propseg;

            /* Compute the number of time quanta in 1 bit time */
            var num_tq = Math.round(peClk_value / ((tmp_presdiv + 1) * bitrate_value * 1000));
            /* Compute the real bitrate resulted */
            var tmp_bitrate = Math.round(peClk_value / ((tmp_presdiv + 1) * num_tq));

            /* The prescaler value became too high, break the loop */
            if (num_tq < min_tq)
            {
                break;
            }

            /* The number of time quanta in 1 bit time must be lower than the one supported */
            if (num_tq <= max_tq) {
                /* Compute time segments based on the value of the sampling point */
                var t_seg1 = Math.round(num_tq * sample_point_value / 100) - 1;
                var t_seg2 = num_tq - 1 - t_seg1;

                /* Adjust time segment 1 and time segment 2 */
                while ((t_seg1 > t_seg1_max) || (t_seg2 < 2))
                {
                    t_seg2 += 1;
                    t_seg1 -= 1;
                }

                tmp_pseg2 = t_seg2 - 1;

                /* Start from pseg1 = pseg2 and adjust until propseg is valid */
                tmp_pseg1 = tmp_pseg2;
                if (fd_enable == false)
                    tmp_propseg = t_seg1 - tmp_pseg1 - 2;
                else
                    tmp_propseg = t_seg1 - tmp_pseg1 - 1;

                while (tmp_propseg < 0)
                {
                    tmp_propseg += 1;
                    tmp_pseg1 -= 1;
                }
                while (tmp_propseg > propseg_max)
                {
                    tmp_propseg -= 1;
                    tmp_pseg1 += 1;
                }

                if ((t_seg1 > t_seg1_max) || (t_seg2 > t_seg2_max) || (t_seg2 < t_seg2_min) || (t_seg1 < t_seg1_min))
                {
                    continue;
                }
                if ((tmp_propseg > propseg_max) || (tmp_pseg1 > pseg1_max) || (tmp_pseg2 > pseg2_max))
                {
                    continue;
                }

                var tmp_sample = (t_seg1 + 1) * 100 / Math.round(num_tq);
                var dsample = Math.abs(tmp_sample - sample_point_value);
                var dbitrate = Math.abs(tmp_bitrate - bitrate_value * 1000);

                if ((dbitrate < min_dbitrate) || ((dbitrate == min_dbitrate) && (dsample < min_dsample)))
                {
                    min_dsample = dsample;
                    min_dbitrate = dbitrate
                    pseg1 = tmp_pseg1;
                    pseg2 = tmp_pseg2;
                    presdiv = tmp_presdiv;
                    propseg = tmp_propseg;
                    real_bitrate = tmp_bitrate;
                    real_sample = tmp_sample;
                    found_values = true;
                }
            }
        }
        if (found_values)
        {
            result.add(presdiv);
            result.add(pseg1);
            result.add(pseg2);
            result.add(propseg);
            result.add(real_bitrate/1000);
            result.add(real_sample);
        } else {
            result.add(4);
            result.add(4);
            result.add(1);
            result.add(7);
            result.add(500);
            result.add(87.5);
        }
   }
    return result;
   }

   calcBautRate();