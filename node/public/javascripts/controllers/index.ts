/// <reference path='../_app.ts' />

/**
 * Created by omnia on 11/16/15.
 */

module RobotArm {
    export class IndexController {
        public status: string = 'boot';

        //noinspection JSUnusedGlobalSymbols
        public static $inject = ['$mdSidenav', '$http', '$mdDialog', '$timeout', '$interval'];

        constructor(private mdSideNav: any,
                    private http: ng.IHttpService,
                    private mdDialog: any,
                    private timeout: ng.ITimeoutService,
                    private interval: ng.IIntervalService) { }

        public toggleSidePane() {
            this.mdSideNav('left')
                .toggle()
                .then(function () { });
        }

        public moveForward() {
            this.http.post('/api/move',
                {
                    movedirection: 1,
                    centimeters: 5
                }
            ).then((success) => {
                console.log(success);
            }, (error) => {
                this.status = 'error';
                console.log(error);
            });
        }
        public turnLeft() {
            this.http.post('/api/rotate',
                {
                    rotatedirection: 3,
                    degrees: 10
                }
            ).then((success) => {
                console.log(success);
            }, (error) => {
                this.status = 'error';
                console.log(error);
            });
        }
        public turnRight() {
            this.http.post('/api/rotate',
                {
                    rotatedirection: 4,
                    degrees: 10
                }
            ).then((success) => {
                console.log(success);
            }, (error) => {
                this.status = 'error';
                console.log(error);
            });
        }
        public moveBackward() {
            this.http.post('/api/move',
                {
                    movedirection: 2,
                    centimeters: 5
                }
            ).then((success) => {
                console.log(success);
            }, (error) => {
                this.status = 'error';
                console.log(error);
            });
        }
        public openClaw() {
            this.http.post('/api/claw',
                {
                    centimeters: 5
                }
            ).then((success) => {
                console.log(success);
            }, (error) => {
                this.status = 'error';
                console.log(error);
            });
        }
        public closeClaw() {
            this.http.post('/api/claw',
                {
                    centimeters: 3
                }
            ).then((success) => {
                console.log(success);
            }, (error) => {
                this.status = 'error';
                console.log(error);
            });
        }
        public raiseArm() {
            this.http.post('/api/arm',
                {
                    centimeters: 15
                }
            ).then((success) => {
                console.log(success);
            }, (error) => {
                this.status = 'error';
                console.log(error);
            });
        }
        public lowerArm() {
            this.http.post('/api/arm',
                {
                    centimeters: 5
                }
            ).then((success) => {
                console.log(success);
            }, (error) => {
                this.status = 'error';
                console.log(error);
            });
        }
    }

    angular.module('RobotArm').controller('IndexController', IndexController);
}