#include "simulation/Elements.h"
 Element_LSNS::Element_LSNS()
 {
	Identifier = "DEFAULT_PT_LSNS";
 	Name = "LSNS";
 	Colour = PIXPACK(0x336699);
 	MenuVisible = 1;
 	MenuSection = SC_SENSOR;
 	Enabled = 1;
 
 	Advection = 0.0f;
 	AirDrag = 0.00f * CFDS;
 	AirLoss = 0.96f;
 	Loss = 0.00f;
 	Collision = 0.0f;
 	Gravity = 0.0f;
 	Diffusion = 0.00f;
 	HotAir = 0.000f	* CFDS;
 	Falldown = 0;
 
 	Flammable = 0;
 	Explosive = 0;
 	Meltable = 0;
 	Hardness = 1;
 
 	Weight = 100;
 
 	Temperature = R_TEMP+0.0f	+273.15f;
 	HeatConduct = 0;
 	Description = "Life sensor. Creates a spark when the life is higher than the temp.";
 
 	Properties = TYPE_SOLID;
 
 	LowPressure = IPL;
 	LowPressureTransition = NT;
 	HighPressure = IPH;
 	HighPressureTransition = NT;
 	LowTemperature = ITL;
 	LowTemperatureTransition = NT;
 	HighTemperature = ITH;
 	HighTemperatureTransition = NT;
 
 	Update = &Element_LSNS::update;
 }
int Element_LSNS::update(UPDATE_FUNC_ARGS)
{
	int r, rx, ry, rt, rd = parts[i].tmp2;
	if (rd > 25) parts[i].tmp2 = rd = 25;
	if (parts[i].life)
	{
		parts[i].life = 0;
		for (rx=-2; rx<3; rx++)
			for (ry=-2; ry<3; ry++)
				if (BOUNDS_CHECK && (rx || ry))
				{
					r = pmap[y+ry][x+rx];
					if (!r)
						continue;
					rt = r&0xFF;
					if (sim->parts_avg(i,r>>8,PT_INSL) != PT_INSL)
					{
						if ((sim->elements[rt].Properties&PROP_CONDUCTS) && !(rt==PT_WATR||rt==PT_SLTW||rt==PT_NTCT||rt==PT_PTCT||rt==PT_INWR) && parts[r>>8].life==0)
						{
							parts[r>>8].life = 4;
							parts[r>>8].ctype = rt;
							sim->part_change_type(r>>8,x+rx,y+ry,PT_SPRK);
						}
					}
				}
	}
	for (rx=-rd; rx<rd+1; rx++)
		for (ry=-rd; ry<rd+1; ry++)
			if (x+rx>=0 && y+ry>=0 && x+rx<XRES && y+ry<YRES && (rx || ry))
			{
				r = pmap[y+ry][x+rx];
				if(!r)
					r = sim->photons[y+ry][x+rx];
				if(!r)
					continue;
				if (parts[r>>8].life > parts[i].temp)
					parts[i].life = 1;
			}
	return 0;
}


Element_LSNS::~Element_LSNS() {}