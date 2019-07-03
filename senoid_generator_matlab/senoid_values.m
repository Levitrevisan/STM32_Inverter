max_pwm_value = 3550;
min_pwm_value = 0;
frequency = 60;

n_of_points_semi_cycle = 33;


i = 0:33;
%%

t = 1/120/33;
disp('period: ')
disp(t)
t = t.*i;

%%
wave = max_pwm_value*sin(2*pi*frequency*t);
wave = cast(max_pwm_value*sin(2*pi*frequency.*t),'uint16');
stem(wave);

%%

string_c = '{';

for i = 1:length(wave) - 1 
    string_c = strcat(string_c,int2str(wave(i)));
    string_c = strcat(string_c,' , ');
end
string_c = strcat(string_c,'}');
disp(string_c)