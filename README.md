# Projet de gestion d'un terrain de golf.

## Créateur

  ##### EL HAROUI Jassim
  ##### GASMI Zakaria
  ##### TONZAR Mohammed
  
## Site d'hébergement 

http://212.115.110.52:1880/ui

## Description

<p>Dans ce projet, on souhaite gérer un terrain de golf en utilisant les objets connectés, en effet on dispose de thermométres, Photoresistor,
ainsi que des robinets d'arrosage (LED).<p>
  
<p> Pour cela, on a un terrain avec diverses qualités de terres, de la plus médiocre avec un indice de 1 a la plus fertile avec un indice de 4, on souhaite gérer cette arrosage en fonction de la météo journaliére, de l'ensoleillement de la zone ainsi que la température global.
De méme on utilise un API Rest de météo pour programmer le l'arrosage par defaut. <p>
  
  ![Schéma](/img/Terrin_golf.PNG)
  
<p> Pour la partie technique on utilisera un ESP32 auquel on y connéctera nos objets, Node Red pour creer des workflows et ainsi aider a mieux automatiser et sert aussi a envoyer des alertes aux abbonées, via un email pour prévenir de la mauvaise météo à venir. <p>
  
<p>On utilisera la base de données Mongodb afin d'enregistrer les differents données (états des capteurs a des moments donnés, arrosage, températures), en utilisant Node-red <p>
<p> On veut aussi mettre en place un dashboard qui illustre le télémetrie provenant des capteurs, (température, lumiére, arrosage )<p>

![Schéma](/img/shema.PNG)
