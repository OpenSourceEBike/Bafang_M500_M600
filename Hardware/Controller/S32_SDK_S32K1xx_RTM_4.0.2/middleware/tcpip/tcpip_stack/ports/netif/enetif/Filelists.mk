NETIFDRVFILES=$(NETIFDIR)/enetif.c \
    $(SDKDIR)/platform/drivers/src/enet/enet_driver.c \
    $(SDKDIR)/platform/drivers/src/enet/enet_irq.c

ifeq ($(CPU),$(filter $(CPU),MPC5777C MPC5746R))
NETIFDRVFILES+=$(SDKDIR)/platform/drivers/src/enet/fec_hw_access.c
else
NETIFDRVFILES+=$(SDKDIR)/platform/drivers/src/enet/enet_hw_access.c
endif

enetif.o: $(NETIFDIR)/enetif.c
	$(CC) $(CFLAGS) $(CGCOVFLAG) -DDEV_ERROR_DETECT -Wno-error -Wno-redundant-decls -c $(<:.o=.c)

enet_driver.o: $(SDKDIR)/platform/drivers/src/enet/enet_driver.c
	$(CC) $(CFLAGS) -DDEV_ERROR_DETECT -c $(<:.o=.c)

ifeq ($(CPU),$(filter $(CPU),MPC5777C MPC5746R))
fec_hw_access.o: $(SDKDIR)/platform/drivers/src/enet/fec_hw_access.c
	$(CC) $(CFLAGS) -DDEV_ERROR_DETECT -c $(<:.o=.c)
else
enet_hw_access.o: $(SDKDIR)/platform/drivers/src/enet/enet_hw_access.c
	$(CC) $(CFLAGS) -DDEV_ERROR_DETECT -c $(<:.o=.c)
endif

enet_irq.o: $(SDKDIR)/platform/drivers/src/enet/enet_irq.c
	$(CC) $(CFLAGS) -DDEV_ERROR_DETECT -c $(<:.o=.c)
