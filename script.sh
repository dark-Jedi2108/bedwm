#!/bin/bash
PROMPT=$1
# TODO -> eww keybinds , post install setup
checkdir() {
  if [ -d $1 ]; then
	  echo "$1 exists"
  else 
    mkdir $1
  fi
}

riceone() { 
  echo "Applying $1"
  # Change dwm
  cp ~/bedwm/suckless/dwm/themes/$1.h ~/bedwm/suckless/dwm/themes/current.h
  cd ~/bedwm/suckless/dwm
  sudo make clean install

  # Change st
  cp ~/bedwm/suckless/st/themes/$1.h ~/bedwm/suckless/st/themes/current.h
  cd ~/bedwm/suckless/st
  sudo make clean install

  # Change rofi 
  cp ~/.local/share/rofi/themes/$1.rasi ~/.local/share/rofi/themes/current.rasi

  # change slstauts
  
  cp ~/bedwm/suckless/slstatus/themes/$1.h ~/bedwm/suckless/slstatus/themes/current.h
  cd ~/bedwm/suckless/slstatus
  sudo make clean install

  #dunst
  cp ~/.config/dunst/$1dunst ~/.config/dunst/dunstrc

  # gtk
  cp ~/.config/gtk-3.0/$1.ini ~/.config/gtk-3.0/settings.ini

  #eww
  cp ~/.config/eww/$1.scss ~/.config/eww/theme.scss
  
  # neovim
  cp ~/.config/nvim/$1.vim ~/.config/nvim/init.vim


  feh --bg-scale ~/bedwm/$1.png
  nvim +PlugInstall
  CURRENT_THEME=$1
  echo "Sucessfull! Now just restart Xorg to see effects!"
}

setup() {
  #sudo pacman -S rofi neovim dunst thunar picom lxappearance firefox discord
  #yay -S eww betterlockscreen visual-studio-code-bin spotify 
  #yay -S nerd-fonts-jetbrains-mono nerd-fonts-fira-code ttf-comic-mono-git #fonts
  #curl -fLo ~/.local/share/nvim/site/autoload/plug.vim --create-dirs https://raw.githubusercontent.com/junegunn/vim-plug/master/plug.vim #vim
  checkdir ~/.config/rofi
  cp -r ~/bedwm/rofi ~/.config
  cp -r ~/bedwm/rofi-themes ~/.local/share/rofi/
  mv ~/.local/share/rofi/rofi-themes ~/.local/share/rofi/themes
  checkdir ~/.config/picom
  cp -r ~/bedwm/picom ~/.config/
  checkdir ~/.config/nvim
  cp -r ~/bedwm/nvim ~/.config/
  checkdir ~/.config/dunst
  cp -r ~/bedwm/dunst  ~/.config/
  checkdir ~/.config/eww
  cp -r ~/bedwm/eww ~/.config/
  checkdir ~/.themes
  cp -r ~/bedwm/gtk-themes/* ~/.themes
  checkdir ~/.config/gtk-3.0
  cp -r ~/bedwm/gtk-3/* ~/.config/gtk-3.0
  touch ~/.xinitrc
  cp ~/bedwm/xinitrc ~/.xinitrc  
}

post() {
  betterlockscreen -u ~/bedwm/lock.png
  feh --bg-scale ~/bedwm/$CURRENT_THEME.png
}

if [ $PROMPT == "gruv" ]; then
  riceone gruvbox
elif [ $PROMPT == 'nord' ]; then
  riceone nord
elif [ $PROMPT == 'dracula' ]; then
  riceone dracula
elif [ $PROMPT == 'setup' ]; then
  setup
elif [ $PROMPT == 'postinst' ]; then
  post
else
  echo "Invalid Theme"
fi


