CREATE TABLE IF NOT EXISTS users (
    userId int NOT NULL AUTO_INCREMENT,
    login varchar(255) NOT NULL,
    pass varchar(255) NOT NULL,
    name varchar(255) NOT NULL,
    role varchar(14) NOT NULL,

    PRIMARY KEY(userId),
    CONSTRAINT UC_Login UNIQUE(login)
);

CREATE TABLE IF NOT EXISTS courses (
    courseId int NOT NULL AUTO_INCREMENT,
    name varchar(255) NOT NULL,
    description varchar(1023) NOT NULL,
    type varchar(255) NOT NULL,
    tags varchar(255) NOT NULL,
    price int NOT NULL,
    approved boolean NOT NULL DEFAULT 0,
    guarantor int NOT NULL,

    PRIMARY KEY(courseId),
    FOREIGN KEY(guarantor) REFERENCES users(userId)
);

CREATE TABLE IF NOT EXISTS lectors (
    lectorId int NOT NULL AUTO_INCREMENT,
    userId int NOT NULL,
    courseId int NOT NULL,

    PRIMARY KEY(lectorId),
    FOREIGN KEY(userId) REFERENCES users(userId),
    FOREIGN KEY(courseId) REFERENCES courses(courseId)
);

CREATE TABLE IF NOT EXISTS terms (
    termId int NOT NULL AUTO_INCREMENT,
    name varchar(255) NOT NULL,
    description varchar(1023) NOT NULL,
    type varchar(255) NOT NULL,
    score int NOT NULL,
    datetime DATETIME NOT NULL,
    courseId int NOT NULL,

    PRIMARY KEY(termId),
    FOREIGN KEY(courseId) REFERENCES courses(courseId)
);

CREATE TABLE IF NOT EXISTS files (
    fileId int NOT NULL AUTO_INCREMENT,
    fileName varchar(255) NOT NULL,
    termId int NOT NULL,

    PRIMARY KEY(fileId),
    FOREIGN KEY(termId) REFERENCES terms(termId) 
);

CREATE TABLE IF NOT EXISTS rooms (
    roomId int NOT NULL AUTO_INCREMENT,
    address varchar(255) NOT NULL,
    doorNumber varchar(255) NOT NULL,
    type varchar(255) NOT NULL,
    capacity int NOT NULL,

    PRIMARY KEY(roomId)
);

CREATE TABLE IF NOT EXISTS termsRooms (
    termRoomId int NOT NULL AUTO_INCREMENT,
    termId int NOT NULL,
    roomId int NOT NULL,

    PRIMARY KEY(termRoomId),
    FOREIGN KEY(termId) REFERENCES terms(termId),
    FOREIGN KEY(roomId) REFERENCES rooms(roomId) 
);

CREATE TABLE IF NOT EXISTS registred (
    registredId int NOT NULL AUTO_INCREMENT,
    approved boolean NOT NULL DEFAULT 0,
    score int DEFAULT 0,
    userId int NOT NULL,
    courseId int NOT NULL,

    PRIMARY KEY(registredId),
    FOREIGN KEY(userId) REFERENCES users(userId),
    FOREIGN KEY(courseId) REFERENCES courses(courseId),
    CONSTRAINT CHK_score CHECK(score BETWEEN 0 AND 100)
);

#ADMIN
INSERT INTO users(userId, name, login, pass, role) VALUES(1, 'Admin Admin', 'Admin', 'aaa', 'administrator');

#SUPERVISORS
INSERT INTO users(userId, name, login, pass, role) VALUES(2, 'Pepa Supervisor', 'SuperP', 'aaa', 'supervisor');
INSERT INTO users(userId, name, login, pass, role) VALUES(3, 'Janek Supervisor', 'SuperJ', 'aaa', 'supervisor');

#GUARANTORS
INSERT INTO users(userId, name, login, pass, role) VALUES(4, 'Karel Guarantor', 'GuarK', 'aaa', 'guarantor');
INSERT INTO users(userId, name, login, pass, role) VALUES(5, 'Bobek Guarantor', 'GuarB', 'aaa', 'guarantor');

#LECTORS
INSERT INTO users(userId, name, login, pass, role) VALUES(6, 'Honza Lector', 'LectH', 'aaa', 'lector');
INSERT INTO users(userId, name, login, pass, role) VALUES(7, 'Jan Lector', 'LectJ', 'aaa', 'lector');

#STUDENTS
INSERT INTO users(userId, name, login, pass, role) VALUES(8, 'Radek Student', 'StudR', 'aaa', 'student');
INSERT INTO users(userId, name, login, pass, role) VALUES(9, 'Pavel Student', 'StudP', 'aaa', 'student');

#COURSES
INSERT INTO courses(courseId, name, description, type, tags, price, guarantor) VALUES(1, 'Course one','One one','type one','tag1,tag2',111,4);
INSERT INTO courses(courseId, name, description, type, tags, price, guarantor) VALUES(2, 'Course two','Two two','type two','tag2,tag3',222,5);
INSERT INTO courses(courseId, name, description, type, tags, price, guarantor) VALUES(3, 'Course three','Three three','type three','tag3',333,4);

#LECTORS
INSERT INTO lectors(lectorId, userId, courseId) VALUES(1, 6, 1);
INSERT INTO lectors(lectorId, userId, courseId) VALUES(2, 6, 2);
INSERT INTO lectors(lectorId, userId, courseId) VALUES(3, 7, 2);
INSERT INTO lectors(lectorId, userId, courseId) VALUES(4, 7, 3);

#TERMS
INSERT INTO terms(termId, name, description, type, score, datetime, courseId) VALUES(1, 'Term one', 'One one', 'Lecture', 10, '2020-01-01 11:11:00', 1);
INSERT INTO terms(termId, name, description, type, score, datetime, courseId) VALUES(2, 'Term two', 'Two two', 'Laboratory', 20, '2020-01-02 11:11:00', 1);
INSERT INTO terms(termId, name, description, type, score, datetime, courseId) VALUES(3, 'Term three', 'Three Three', 'Demo', 30, '2020-01-03 11:11:00', 2);
INSERT INTO terms(termId, name, description, type, score, datetime, courseId) VALUES(4, 'Term four', 'Four four', 'Excersice', 40, '2020-01-04 11:11:00', 2);
INSERT INTO terms(termId, name, description, type, score, datetime, courseId) VALUES(5, 'Term five', 'Five five', 'Demo', 50, '2020-01-05 11:11:00', 3);
INSERT INTO terms(termId, name, description, type, score, datetime, courseId) VALUES(6, 'Term six', 'Six six', 'Lecture', 50, '2020-01-06 11:11:00', 3);

#FILES
INSERT INTO files(fileId, fileName, termId) VALUES(1, '1.txt', 2);
INSERT INTO files(fileId, fileName, termId) VALUES(2, '2.txt', 4);
INSERT INTO files(fileId, fileName, termId) VALUES(3, '3.txt', 6);

#ROOMS
INSERT INTO rooms(roomId, address, doorNumber, type, capacity) VALUES(1, 'Address 1', 'A111', 'Type one', 11);
INSERT INTO rooms(roomId, address, doorNumber, type, capacity) VALUES(2, 'Address 2', 'B222', 'Type two', 22);
INSERT INTO rooms(roomId, address, doorNumber, type, capacity) VALUES(3, 'Address 3', 'C333', 'Type three', 33);

#TERMSROOMS
INSERT INTO termsRooms(termRoomId, termId, roomId) VALUES(1, 1, 3);
INSERT INTO termsRooms(termRoomId, termId, roomId) VALUES(2, 3, 2);
INSERT INTO termsRooms(termRoomId, termId, roomId) VALUES(3, 4, 1);

#REGISTRED
INSERT INTO registred(registredId, userId, courseId) VALUES(1, 8, 3);
INSERT INTO registred(registredId, userId, courseId) VALUES(2, 9, 2);
INSERT INTO registred(registredId, userId, courseId) VALUES(3, 7, 3);
INSERT INTO registred(registredId, userId, courseId) VALUES(4, 5, 1);