const canvas = document.getElementById("editorCanvas");
const ctx = canvas.getContext("2d");

let points = [];
let shapes = [];
let currentFillColor = "#0000ff";  // Цвет по умолчанию для заливки

// Добавляем слушатель для выбора цвета
document.getElementById("colorPicker").addEventListener("input", (event) => {
    currentFillColor = event.target.value;
});

canvas.addEventListener("click", (event) => {
    const rect = canvas.getBoundingClientRect();
    const x = event.clientX - rect.left;
    const y = event.clientY - rect.top;

    const normalizedX = (2 * x / canvas.width) - 1;
    const normalizedY = 1 - (2 * y / canvas.height);

    const newPoint = {
        x: roundToThreeDecimalPlaces(normalizedX),
        y: roundToThreeDecimalPlaces(normalizedY),
        texID: 0,
        texCoords: { u: 0, v: 0 },
        color: { r: 255, g: 0, b: 0, a: 1.0 }
    };

    points.push(newPoint);
    redrawCanvas();
    updatePointList();
});

// Функция округления до трех знаков после запятой
function roundToThreeDecimalPlaces(value) {
    return Math.round(value * 1000) / 1000;
}

function drawPoint(point) {
    ctx.fillStyle = `rgba(${point.color.r}, ${point.color.g}, ${point.color.b}, ${point.color.a})`;
    ctx.beginPath();
    ctx.arc(((point.x + 1) / 2) * canvas.width, ((1 - point.y) / 2) * canvas.height, 5, 0, Math.PI * 2);
    ctx.fill();
}

function drawLine(p1, p2) {
    ctx.strokeStyle = "#000";
    ctx.beginPath();
    ctx.moveTo(((p1.x + 1) / 2) * canvas.width, ((1 - p1.y) / 2) * canvas.height);
    ctx.lineTo(((p2.x + 1) / 2) * canvas.width, ((1 - p2.y) / 2) * canvas.height);
    ctx.stroke();
}

// Функция для нормализации координат текстуры
function calculateTexCoords(shape) {
    let minX = Math.min(...shape.points.map(p => p.x));
    let maxX = Math.max(...shape.points.map(p => p.x));
    let minY = Math.min(...shape.points.map(p => p.y));
    let maxY = Math.max(...shape.points.map(p => p.y));

    shape.points.forEach(point => {
        point.texCoords.u = roundToThreeDecimalPlaces((point.x - minX) / (maxX - minX));  // Нормализуем по X
        point.texCoords.v = roundToThreeDecimalPlaces((point.y - minY) / (maxY - minY));  // Нормализуем по Y
    });
}

function finishShape() {
    if (points.length < 3) {
        alert("A shape must have at least 3 points.");
        return;
    }

    const indices = [];
    for (let i = 1; i < points.length - 1; i++) {
        indices.push(0, i, i + 1);
    }

    const newShape = {
        points: [...points],
        indices: indices,
        color: currentFillColor  // Запоминаем цвет для этой фигуры
    };

    // Автоматически рассчитываем текстурные координаты для этой фигуры
    calculateTexCoords(newShape);

    shapes.push(newShape);
    points = [];
    redrawCanvas();
    updatePointList();
}

function exportData() {
    const data = shapes.map((shape, i) => ({
        name: `Figure ${i + 1}`,
        points: shape.points.map(point => ({
            x: point.x,
            y: point.y,
            texCoords: {
                u: roundToThreeDecimalPlaces(point.texCoords.u),
                v: roundToThreeDecimalPlaces(point.texCoords.v)
            },
            color: point.color
        })),
        indices: shape.indices,
        color: shape.color
    }));

    const dataStr = JSON.stringify(data, null, 2);
    const blob = new Blob([dataStr], { type: "application/json" });
    const url = URL.createObjectURL(blob);
    const a = document.createElement("a");
    a.href = url;
    a.download = "shapes.json";
    document.body.appendChild(a);
    a.click();
    document.body.removeChild(a);
}

function updatePointList() {
    const pointList = document.getElementById("pointList");
    pointList.innerHTML = "";

    shapes.forEach((shape, shapeIndex) => {
        const shapeGroup = document.createElement("div");
        shapeGroup.classList.add("figureGroup");
        shapeGroup.innerHTML = `<h4>Figure ${shapeIndex + 1}</h4>`;

        shape.points.forEach((point, pointIndex) => {
            const pointItem = document.createElement("div");
            pointItem.classList.add("pointItem");
            pointItem.innerHTML = `
                Point ${pointIndex + 1}:
                X: <input type="number" step="0.001" value="${point.x}" onchange="updateShapePoint(${shapeIndex}, ${pointIndex}, 'x', this.value)">
                Y: <input type="number" step="0.001" value="${point.y}" onchange="updateShapePoint(${shapeIndex}, ${pointIndex}, 'y', this.value)">
                Texture ID: <input type="number" value="${point.texID}" onchange="updateShapePoint(${shapeIndex}, ${pointIndex}, 'texID', this.value)">
                U: <input type="number" step="0.001" value="${point.texCoords.u}" onchange="updateShapePointTex(${shapeIndex}, ${pointIndex}, 'u', this.value)">
                V: <input type="number" step="0.001" value="${point.texCoords.v}" onchange="updateShapePointTex(${shapeIndex}, ${pointIndex}, 'v', this.value)">
                R: <input type="number" value="${point.color.r}" onchange="updateShapePointColor(${shapeIndex}, ${pointIndex}, 'r', this.value)">
                G: <input type="number" value="${point.color.g}" onchange="updateShapePointColor(${shapeIndex}, ${pointIndex}, 'g', this.value)">
                B: <input type="number" value="${point.color.b}" onchange="updateShapePointColor(${shapeIndex}, ${pointIndex}, 'b', this.value)">
                A: <input type="number" step="0.1" value="${point.color.a}" onchange="updateShapePointColor(${shapeIndex}, ${pointIndex}, 'a', this.value)">
            `;
            shapeGroup.appendChild(pointItem);
        });

        pointList.appendChild(shapeGroup);
    });
}

function updateShapePoint(shapeIndex, pointIndex, field, value) {
    shapes[shapeIndex].points[pointIndex][field] = roundToThreeDecimalPlaces(parseFloat(value));
    redrawCanvas();
}

function updateShapePointTex(shapeIndex, pointIndex, texField, value) {
    shapes[shapeIndex].points[pointIndex].texCoords[texField] = roundToThreeDecimalPlaces(parseFloat(value));
    redrawCanvas();
}

function updateShapePointColor(shapeIndex, pointIndex, colorField, value) {
    shapes[shapeIndex].points[pointIndex].color[colorField] = parseFloat(value);
    redrawCanvas();
}

function redrawCanvas() {
    ctx.clearRect(0, 0, canvas.width, canvas.height);

    shapes.forEach((shape) => {
        ctx.fillStyle = shape.color;
        ctx.beginPath();
        shape.points.forEach((point, index) => {
            const canvasX = ((point.x + 1) / 2) * canvas.width;
            const canvasY = ((1 - point.y) / 2) * canvas.height;

            if (index === 0) {
                ctx.moveTo(canvasX, canvasY);
            } else {
                ctx.lineTo(canvasX, canvasY);
            }
        });
        ctx.closePath();
        ctx.fill();
    });

    points.forEach((point, index) => {
        drawPoint(point);
        if (index > 0) {
            drawLine(points[index - 1], point);
        }
    });
}

function applyFill() {
    currentFillColor = document.getElementById("colorPicker").value;
    redrawCanvas();
}
