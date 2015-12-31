obj-y += hal.o
obj-y += newlib/
obj-$(CONFIG_UART) += uart/
obj-$(CONFIG_BUFFER) += buffer/
obj-$(CONFIG_SPI) += spi/
obj-$(CONFIG_SENSOR) += sensor/
obj-$(CONFIG_ADC) += adc/
obj-$(CONFIG_TIMER) += timer/
obj-$(CONFIG_PWM) += pwm/
obj-$(CONFIG_CAPTURE) += capture/
obj-y += power/
