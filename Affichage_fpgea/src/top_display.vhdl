library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.NUMERIC_STD.ALL;

entity top_display_mux is
    Port ( 
        clk      : in  STD_LOGIC;                    -- Horloge 100 MHz
        data_in  : in  STD_LOGIC_VECTOR (7 downto 0); -- Résultat 8 bits (SW7..SW0)
        carry_in : in  STD_LOGIC;                    -- Retenue (ex: BTNC)
        seg      : out STD_LOGIC_VECTOR (6 downto 0); -- Segments CA..CG
        dp       : out STD_LOGIC;                    -- Point décimal
        an       : out STD_LOGIC_VECTOR (3 downto 0)  -- Anodes AN3..AN0
    );
end top_display_mux;

architecture Behavioral of top_display_mux is

    -- Déclaration du composant existant
    component hex_to_7seg
        Port ( 
            sw  : in  STD_LOGIC_VECTOR (3 downto 0);
            seg : out STD_LOGIC_VECTOR (6 downto 0);
            dp  : out STD_LOGIC
        );
    end component;

    -- Signaux internes
    signal counter        : unsigned(16 downto 0) := (others => '0');
    signal digit_select   : STD_LOGIC_VECTOR(1 downto 0);
    signal current_nibble : STD_LOGIC_VECTOR(3 downto 0);

    -- Signaux BCD (Renommés pour éviter le mot réservé 'units')
    signal val_data       : integer range 0 to 255;
    signal unit_digit     : STD_LOGIC_VECTOR(3 downto 0);
    signal ten_digit      : STD_LOGIC_VECTOR(3 downto 0);
    signal hundred_digit  : STD_LOGIC_VECTOR(3 downto 0);
    signal carry_digit    : STD_LOGIC_VECTOR(3 downto 0);

begin

    -- 1. Conversion de data_in en BCD (Unités, Dizaines, Centaines)
    val_data      <= to_integer(unsigned(data_in));
    unit_digit    <= std_logic_vector(to_unsigned(val_data mod 10, 4));
    ten_digit     <= std_logic_vector(to_unsigned((val_data / 10) mod 10, 4));
    hundred_digit <= std_logic_vector(to_unsigned(val_data / 100, 4));
    carry_digit   <= "000" & carry_in;

    -- 2. Diviseur de fréquence pour le balayage (~760 Hz)
    process(clk)
    begin
        if rising_edge(clk) then
            counter <= counter + 1;
        end if;
    end process;

    digit_select <= std_logic_vector(counter(16 downto 15));

    -- 3. Multiplexage des anodes
    process(digit_select, unit_digit, ten_digit, hundred_digit, carry_digit)
    begin
        case digit_select is
            when "00" => 
                an <= "1110"; -- Allume Digit 0 (droite : Unités)
                current_nibble <= unit_digit;
            when "01" => 
                an <= "1101"; -- Allume Digit 1 (Dizaines)
                current_nibble <= ten_digit;
            when "10" => 
                an <= "1011"; -- Allume Digit 2 (Centaines)
                current_nibble <= hundred_digit;
            when "11" => 
                an <= "0111"; -- Allume Digit 3 (gauche : Retenue)
                current_nibble <= carry_digit;
            when others => 
                an <= "1111";
                current_nibble <= "0000";
        end case;
    end process;

    -- 4. Instanciation du décodeur 7 segments
    u_decoder: hex_to_7seg
        port map (
            sw  => current_nibble,
            seg => seg,
            dp  => dp
        );

end Behavioral;