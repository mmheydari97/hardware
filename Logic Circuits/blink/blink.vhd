 library IEEE;
 use IEEE.STD_LOGIC_1164.ALL;
 use IEEE.numeric_std.all;
 
 entity led is
    Port ( 
           refclk : in  std_logic;
           led : out std_logic
    );
 end led;
 
 architecture RTL of led is
    constant max_count : natural := 32000000;
    signal Rst : std_logic;
 begin

    Rst <= '0';
 
    -- 0 to max_count counter
    compteur : process(refclk, Rst)
        variable count : natural range 0 to max_count;
    begin
        if Rst = '1' then
            count := 0;
            led <= '1';
        elsif rising_edge(refclk) then
            if count < max_count/2 then
                count := count + 1;
                led <= '1';
            elsif count < max_count then
                led <= '0';
                count := count + 1;
            else
                led <= '1';
                count := 0;
            end if;
        end if;
    end process compteur; 
 end RTL;