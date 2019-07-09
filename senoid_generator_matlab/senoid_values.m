
%%
clear;clc;
max_pwm_value = 3550;
min_pwm_value = 0;
frequency = 60;
number_of_points_in_one_cycle = 60;
%%
i = 0:number_of_points_in_one_cycle;
t = 1/60/number_of_points_in_one_cycle;
fprintf('interruption period: %.2f nandoseconds\n', t*10^9)
t = t.*i;

%%
wave = (max_pwm_value/2)*sin(2*pi*frequency.*t)+(max_pwm_value/2);
wave = cast(wave,'uint16');
stem(wave);

%%

string_c = '{';

for i = 1:length(wave) - 1 
    string_c = strcat(string_c,int2str(wave(i)));
    string_c = strcat(string_c,' , ');
end
string_c = strcat(string_c,'}');
disp('senoid values array')
disp(string_c)