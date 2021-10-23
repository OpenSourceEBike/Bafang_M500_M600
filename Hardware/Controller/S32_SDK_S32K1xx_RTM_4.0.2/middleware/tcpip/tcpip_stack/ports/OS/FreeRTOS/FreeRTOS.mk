FREERTOS_ARCH=$(COMP)/$(OS_ARCH)
FREERTOSDIR=$(call TO_NATIVE,$(FREERTOS_DIR))
FREERTOSFILES=$(FREERTOSDIR)/croutine.c \
	$(FREERTOSDIR)/event_groups.c \
	$(FREERTOSDIR)/list.c \
	$(FREERTOSDIR)/queue.c \
	$(FREERTOSDIR)/tasks.c \
	$(FREERTOSDIR)/timers.c \
	$(FREERTOSDIR)/portable/$(FREERTOS_ARCH)/port.c \
	$(FREERTOSDIR)/portable/MemMang/heap_4.c

ifneq ($(wildcard $(FREERTOSDIR)/portable/$(FREERTOS_ARCH)/portasm.s),)
FREERTOSFILES+=$(FREERTOSDIR)/portable/$(FREERTOS_ARCH)/portasm.s
endif

%.o: $(FREERTOSDIR)/%.c
	$(CC) $(CFLAGS) -c $(<:.o=.c)

%.o: $(FREERTOSDIR)/portable/MemMang/%.c
	$(CC) $(CFLAGS) -c $(<:.o=.c)

%.o: $(FREERTOSDIR)/portable/$(FREERTOS_ARCH)/%.c
	$(CC) $(CFLAGS) -c $(<:.o=.c)

%.o: $(FREERTOSDIR)/portable/$(FREERTOS_ARCH)/%.s
	$(CC) $(CFLAGS) -x assembler-with-cpp -mregnames -c -I$(DEMODIR) -c $(<:.o=.s)



