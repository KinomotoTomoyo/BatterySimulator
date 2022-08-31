
if(solidRegions[i].name() == "anode")
{
  //if voltage is lower than 0V, toggle flag_voltage
  if(min(theta).value() < -1.0e-5) 
  {flag_SOC_a = -1;}

  if(max(theta).value() >= 1) 
  {flag_SOC_a = 1;}

}

else if(solidRegions[i].name() == "cathode")
{
  //if voltage is lower than 0V, toggle flag_voltage
  if(min(theta).value() < -1.0e-5) 
  {flag_SOC_c = -1;}

  if(max(theta).value() >= 1) 
  {flag_SOC_c = 1;}

}


  //if voltage is lower than 0V, toggle flag_voltage
  if(V < 0) 
  {flag_voltage = 1;}





