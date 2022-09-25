sudo rm financialplanner.deb
sudo dpkg -r financialplanner
sudo dpkg --build stagedir financialplanner.deb
sudo dpkg -i financialplanner.deb
sudo rm financialplanner.deb
