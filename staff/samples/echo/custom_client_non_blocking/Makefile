include ../../../Makefile.env

TARGET = echo_custom_client_non_blocking
TARGETNAME = $(TARGET)

CXXFLAGS += -Wno-deprecated -fexceptions -I$(DEPLOYDIR)include -I$(STAFF_HOME)/include/
LDFLAGS += -fexceptions -L$(DEPLOYDIR)lib -L$(STAFF_HOME)/lib
LDFLAGS += -lstaffcommon -lstaffclient -lrise

VPATH = $(subst $(empty) $(empty),:,$(SRCDIR))

SOURCES = $(wildcard $(SRCDIR)*.cpp)
OBJECTS = $(patsubst %.cpp,$(OBJDIR)%.o,$(notdir $(SOURCES)))

# == make ===========================================
make: check "$(OBJDIR)" "$(OUTDIR)" $(OUTDIR)$(TARGETNAME) deploy

# link
$(OUTDIR)$(TARGETNAME): $(OBJECTS)
	$(CXX) $(LDFLAGS) $(OBJECTS) -o $(OUTDIR)$(TARGETNAME)

# compile
$(OBJDIR)%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# == deploy  ========================================
deploy: "$(DEPLOYDIR)$(SAMPLESDIR)$(BINDIR)" $(OUTDIR)$(TARGETNAME)
	cp -f $(OUTDIR)$(TARGETNAME) "$(DEPLOYDIR)$(SAMPLESDIR)$(BINDIR)"

# == distrib =========================================
distrib:;

# == install ========================================
install: check "$(INSTALLDIR)$(SAMPLESDIR)$(BINDIR)"
	cp -f $(OUTDIR)$(TARGETNAME) $(INSTALLDIR)$(SAMPLESDIR)$(BINDIR)$(TARGETNAME)

# == uninstall ======================================
uninstall: check
	rm -f $(INSTALLDIR)$(SAMPLESDIR)$(BINDIR)$(TARGETNAME)

# == clean ==========================================
clean:
	rm -Rf $(OBJDIR) $(OUTDIR)

# == mkdir ==========================================
"%/":
	@[ -z "$@" -o -d "$@" ] || mkdir -p $@ && chmod g+w $@