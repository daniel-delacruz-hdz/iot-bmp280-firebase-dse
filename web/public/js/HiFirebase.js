// Configuracion de Firebase para el proyecto iot-2178079
const firebaseConfig = {
  apiKey: "AIzaSyC461YuBRxh2H-NO98vOYwGgyOocw4Q0zw",
  authDomain: "iot-2178079.firebaseapp.com",
  databaseURL: "https://iot-2178079-default-rtdb.firebaseio.com",
  projectId: "iot-2178079",
  storageBucket: "iot-2178079.firebasestorage.app",
  messagingSenderId: "597985163599",
  appId: "1:597985163599:web:04a3f442e00c5b0c61ad3a"
};


firebase.initializeApp(firebaseConfig);
const estado = document.getElementById('estado');
const resultados = {};
function actualizarEstado() {
  const estados = Object.values(resultados);
  estado.textContent = estados.includes('error') ? 'No se pudieron cargar algunas lecturas.'
    : estados.includes('vacio') ? 'Hay mediciones sin datos disponibles.'
    : estados.length === 3 ? 'Lecturas recibidas de la estación.'
    : 'Cargando mediciones…';
}
[['Temperatura', 'TemperaturaId'], ['Presion', 'PresionId'], ['Altitud', 'AltitudId']].forEach(([ruta, id]) => {
  const elemento = document.getElementById(id);
  firebase.database().ref('Weather/' + ruta).on('value', snapshot => {
    const valor = snapshot.val();
    const valido = (typeof valor === 'number' || (typeof valor === 'string' && valor.trim() !== '')) && Number.isFinite(Number(valor));
    elemento.textContent = valido ? Number(valor).toLocaleString('en-US', { minimumFractionDigits: 2, maximumFractionDigits: 2, useGrouping: false, roundingMode: 'trunc' }) : '—';
    resultados[ruta] = valido ? 'ok' : 'vacio';
    actualizarEstado();
  }, () => {
    elemento.textContent = '—';
    resultados[ruta] = 'error';
    actualizarEstado();
  });
});
