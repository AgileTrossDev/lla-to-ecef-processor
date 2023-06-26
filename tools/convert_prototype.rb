WGS84_A = 6378137.0;             # Semi-major axis 
WGS84_B = 6356752.31424518;      # Semi-minor axis 
WGS84_E = 0.08181919084262149;   # first eccentricity --> Math.sqrt( (a**2 - b**2)/a**2 )
PI = 3.141592653589793238
DEGREES_TO_RADIANS = PI / 180.0
def build_from_lla(lat,  long,  alt) 
  clat = Math.cos(lat)
  slat = Math.sin(lat)
  clong = Math.cos(long)
  slong = Math.sin(long)
  alt = alt/1000

  # Radius of Curvature (meters)
  n = calculate_curvature_from_lat(lat)

  # X = ( N + h ) cos(latitude) cos(longitude)
  x = (n + alt) * clat * clong
  y = (n + alt) * clat * slong
  z = ((n * (WGS84_B**2/WGS84_A**2) ) + alt) * slat

  puts "RESULT: #{x} #{y}  #{z}"

end


def calculate_curvature_from_lat(lat)
  
  rad_lat = lat * DEGREES_TO_RADIANS
  slat = Math.sin(rad_lat)  
  slat2 = slat**2  
  e2 = WGS84_E**2  
  puts "LAT: #{lat} RAD: #{rad_lat} SIN: #{slat} SIN_SQUARE: #{slat2} e2: #{e2} SQR: #{Math.sqrt(1.0 - ( e2 * slat2 * lat))} | #{WGS84_A}"

  ret_val = WGS84_A / (Math.sqrt(1.0 - e2 * slat2))

  return ret_val
end


build_from_lla(40.3057447, -76.0548441, 0.06)

puts "Expecting:  1173832.66, -4727257.98, 4103933.82"


puts calculate_curvature_from_lat(0)
puts calculate_curvature_from_lat(5)
#puts calculate_curvature_from_lat(10)
#puts calculate_curvature_from_lat(15)
#puts calculate_curvature_from_lat(20)
