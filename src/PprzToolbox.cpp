#include "PprzToolbox.h"
#include "AircraftManager.h"

PprzToolbox::PprzToolbox(PprzApplication* app)
{
    _aircraftManager = new AircraftManager(app, this);
}

void PprzToolbox::setChildToolboxes(void) {
    _aircraftManager->setToolbox(this);
}


PprzTool::PprzTool(PprzApplication* app, PprzToolbox* toolbox) : _app(app), _toolbox(nullptr)
{
    (void)toolbox;
}
void PprzTool::setToolbox(PprzToolbox* toolbox)
{
    _toolbox = toolbox;
}
