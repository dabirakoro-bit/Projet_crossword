# Module d'affichage 4 Digits Multiplexé (Nexys 3)

## Description

Composant VHDL modulaire permettant d'afficher une valeur 8 bits (0 à 255) sur 3 digits (centaines, dizaines, unités) et une retenue 1 bit sur le digit de gauche.

## Architecture VHDL

* **`hex\_to\_7seg.vhd`** : Décodeur unitaire 4 bits vers 7 segments (Anode commune, logique négative).
* **`top\_display\_mux.vhd`** : Top-level gérant le diviseur d'horloge (\~760 Hz), la conversion BCD et le balayage des 4 anodes.

## Entrées / Sorties (`top\_display\_mux`)

|Port|Sens|Taille|Description|
|-|-|-|-|
|`clk`|Entrée|1 bit|Horloge système (100 MHz - Pin V10)|
|`data\_in`|Entrée|8 bits|Donnée binaire à afficher (0 à 255)|
|`carry\_in`|Entrée|1 bit|Retenue d'opération (affichée sur AN3)|
|`seg`|Sortie|7 bits|Bus des segments (CA à CG - Active à '0')|
|`dp`|Sortie|1 bit|Point décimal (Forcé à '1' = éteint)|
|`an`|Sortie|4 bits|Bus de sélection des anodes (AN3 à AN0 - Active à '0')|

## Découpage des digits

* **AN3 (Digit 3 - Gauche)** : Retenue (`0` ou `1`)
* **AN2 (Digit 2)** : Centaines (`0` à `2`)
* **AN1 (Digit 1)** : Dizaines (`0` à `9`)
* **AN0 (Digit 0 - Droite)** : Unités (`0` à `9`)

## Réutilisation dans un autre projet

1. Ajouter `hex\_to\_7seg.vhd` et `top\_display\_mux.vhd` au projet ISE.
2. Instancier `top\_display\_mux` dans le composant principal (ex: processeur ou ALU).
3. Associer le fichier `.ucf` fourni pour mapper l'horloge et les afficheurs.


-- Instanciation de l'afficheur dans ton projet d'ALU / CPU
u_display : entity work.top_display_mux
    port map (
        clk      => clk_100MHz,
        data_in  => alu_result, -- Ton bus de résultat 8 bits
        carry_in => alu_carry,  -- Ta retenue de sortie
        seg      => seg,        -- Relié aux ports physiques
        dp       => dp,
        an       => an
    );
