library IEEE;
use IEEE.STD_LOGIC_1164.ALL;

entity hex_to_7seg is
    Port ( 
        sw  : in  STD_LOGIC_VECTOR (3 downto 0); -- sw(3)=SW3, sw(0)=SW0
        seg : out STD_LOGIC_VECTOR (6 downto 0); -- seg(6)=CA, ..., seg(0)=CG
        dp  : out STD_LOGIC;                    -- Point décimal
        an  : out STD_LOGIC_VECTOR (3 downto 0)  -- Anodes
    );
	
	end hex_to_7seg;

architecture Behavioral of hex_to_7seg is
begin
    -- Activation du premier digit (droite) : AN0 à '0' (anode commune)
    an <= "1110"; 

    -- Éteindre le point décimal (actif à '0')
    dp <= '1';
	 -- Décodeur hexadécimal vers 7 segments
    -- Ordre des bits : (CA, CB, CC, CD, CE, CF, CG)
    -- '0' = segment allumé, '1' = segment éteint
    process(sw)
    begin
	 case sw is
            when "0000" => seg <= "0000001"; -- 0 (tout allumé sauf G)
            when "0001" => seg <= "1001111"; -- 1 (B, C)
            when "0010" => seg <= "0010010"; -- 2 (A, B, D, E, G)
            when "0011" => seg <= "0000110"; -- 3 (A, B, C, D, G)
            when "0100" => seg <= "1001100"; -- 4 (B, C, F, G)
            when "0101" => seg <= "0100100"; -- 5 (A, C, D, F, G)
            when "0110" => seg <= "0100000"; -- 6 (A, C, D, E, F, G)
            when "0111" => seg <= "0001111"; -- 7 (A, B, C)
            when "1000" => seg <= "0000000"; -- 8 (Tous allumés)
            when "1001" => seg <= "0000100"; -- 9 (A, B, C, D, F, G)
				when "1010" => seg <= "0001000"; -- A
            when "1011" => seg <= "1100000"; -- b
            when "1100" => seg <= "0110001"; -- C
            when "1101" => seg <= "1000010"; -- d
            when "1110" => seg <= "0110000"; -- E
            when "1111" => seg <= "0111000"; -- F
				when others => seg <= "1111111";
        end case;
    end process;

end Behavioral;