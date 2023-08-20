EXECUTABLE = triangles
CC = g++
CFLAGS = -Wall -O2
LDFLAGS = -lGL -lGLU -lglut
SOURCES = main.cpp
OBJDIR = build
DEPDIR = $(OBJDIR)
OBJECTS = $(addprefix $(OBJDIR)/, $(patsubst %, %.o, $(basename $(SOURCES))))
DEPFLAGS = -MT $@ -MMD -MP -MF $(DEPDIR)/$*.Td
MOVEDEPS = mv -f $(DEPDIR)/$*.Td $(DEPDIR)/$*.d
$(shell mkdir -p $(OBJDIR) > /dev/null)

all: $(OBJDIR)/$(EXECUTABLE)

%.o: %.cpp

$(OBJDIR)/%.o: %.cpp $(DEPDIR)/%.d
	$(CC) $(DEPFLAGS) $(CFLAGS) -c -o $@ $<
	$(MOVEDEPS)

$(OBJDIR)/$(EXECUTABLE): $(OBJECTS)
	$(CC) $(CFLAGS) -o $@ $(OBJECTS) $(LDFLAGS)

$(DEPDIR)/%.d: ;

# do not remove .d files
.PRECIOUS: $(DEPDIR)/%.d

include $(wildcard $(patsubst %, $(DEPDIR)/%.d, $(basename $(SOURCES))))

.PHONY: clean

clean:
	-rm -f $(OBJDIR)/$(EXECUTABLE) $(OBJECTS)
	-rm -f $(wildcard $(patsubst %, $(DEPDIR)/%.d, $(basename $(SOURCES))))

